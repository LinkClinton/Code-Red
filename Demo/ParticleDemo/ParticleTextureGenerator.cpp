#include "ParticleTextureGenerator.hpp"

ParticleTextureGenerator::ParticleTextureGenerator(
	const std::shared_ptr<CodeRed::GpuLogicalDevice>& device,
	const std::shared_ptr<CodeRed::GpuCommandAllocator>& allocator,
	const size_t detailLevel,
	const size_t detailSize) :
	mDetailLevel(detailLevel), mDetailSize(detailSize),
	mDevice(device), mCommandAllocator(allocator)
{
	initializeCommands();
	initializeBuffers();
	initializeShaders();
	initializeTextures();
	initializePipeline();
}

void ParticleTextureGenerator::run() const
{
	//the color we used to clear the render target
	//because we want to build a particle texture
	//the pixel not in the circle is (0, 0, 0, 0)
	//we will use blend or clip to ignore this pixel
	auto renderPass = mPipelineInfo->graphicsPipeline()->renderPass();

	renderPass->setClear(CodeRed::ClearValue(0.0f, 0.0f, 0.0f, 0.0f));
	
	mCommandList->beginRecoding();

	//set graphics pipeline, resource layout and frame buffer
	mCommandList->setGraphicsPipeline(mPipelineInfo->graphicsPipeline());
	mCommandList->setResourceLayout(mPipelineInfo->graphicsPipeline()->layout());
	
	mCommandList->setViewPort(mFrameBuffer->fullViewPort());
	mCommandList->setScissorRect(mFrameBuffer->fullScissorRect());

	//set vertex, index, view buffer
	mCommandList->setVertexBuffer(mVertexBuffer);
	mCommandList->setIndexBuffer(mIndexBuffer);
	mCommandList->setGraphicsConstantBuffer(0, mViewBuffer);

	mCommandList->beginRenderPass(renderPass, mFrameBuffer);
	
	//draw particle to texture
	mCommandList->drawIndexed(mIndexBuffer->count());

	mCommandList->endRenderPass();
	
	mCommandList->endRecoding();

	//execute commands
	mCommandQueue->execute({ mCommandList });
}

void ParticleTextureGenerator::waitFinish() const
{
	mCommandQueue->waitIdle();
}

void ParticleTextureGenerator::initializeCommands()
{
	//create the graphics command list and command queue
	mCommandList = mDevice->createGraphicsCommandList(mCommandAllocator);
	mCommandQueue = mDevice->createCommandQueue();
}

void ParticleTextureGenerator::initializeBuffers()
{
	//build the particle shape, it is a circle with ("mDetailLevel") vertices
	//becasue we only use it once, we create it on upload heap instead of default heap
	mVertexBuffer = mDevice->createBuffer(
		CodeRed::ResourceInfo::VertexBuffer(
			sizeof(ParticleVertex),
			mDetailLevel + 1,
			CodeRed::MemoryHeap::Upload
		)
	);

	mIndexBuffer = mDevice->createBuffer(
		CodeRed::ResourceInfo::IndexBuffer(
			sizeof(unsigned),
			mDetailLevel * 3,
			CodeRed::MemoryHeap::Upload
		)
	);

	//view buffer is a matrix used for projection
	//we use glm::orthoLH_ZO() with "mDetailSize"
	mViewBuffer = mDevice->createBuffer(
		CodeRed::ResourceInfo::ConstantBuffer(
			sizeof(glm::mat4x4)
		)
	);

	std::vector<ParticleVertex> vertices(mDetailLevel + 1);
	std::vector<unsigned> indices(mDetailLevel * 3);

	//the center of circle is (0, 0), so the texcoord is (0, 0).
	vertices[0] = ParticleVertex();

	for (size_t index = 1; index < vertices.size(); index++) {
		//we divide the arc of circle into mDetailLevel sub-arcs
		const auto arc = glm::two_pi<float>() / (mDetailLevel - 1) * (index - 1);
		//and we use complex coordinate system to describe the vertex buffer
		auto complex_positon = glm::vec2(glm::cos(arc), glm::sin(arc));

		//build the vertices data, the position and the texcoord is same
		vertices[index] = ParticleVertex(
			complex_positon,
			complex_positon
		);

		//build the indices data
		indices[(index - 1) * 3 + 0] = 0;
		indices[(index - 1) * 3 + 1] = static_cast<unsigned>(index);
		indices[(index - 1) * 3 + 2] = static_cast<unsigned>(index + 1 == vertices.size() ? 0 : index + 1);
	}

	//the view space is a square with size 1, center (0, 0)
	auto view = glm::orthoLH_ZO(-1.0f, 1.0f, 1.0f, -1.0f, 0.0f, 1.0f);

	//map memory, copy data and unmap
	//so we finish to initialize buffers
	const auto vertexMemory = mVertexBuffer->mapMemory();
	const auto indexMemory = mIndexBuffer->mapMemory();
	const auto viewMemory = mViewBuffer->mapMemory();

	std::memcpy(vertexMemory, vertices.data(), mVertexBuffer->size());
	std::memcpy(indexMemory, indices.data(), mIndexBuffer->size());
	std::memcpy(viewMemory, &view, mViewBuffer->size());
	
	mVertexBuffer->unmapMemory();
	mIndexBuffer->unmapMemory();
	mViewBuffer->unmapMemory();
}

void ParticleTextureGenerator::initializeShaders()
{
#ifdef __DIRECTX12__MODE__
	//in DirectX 12 mode, we use HLSL and compile them with D3DCompile
	static const auto vertexShaderText =
		"#pragma pack_matrix(row_major)\n"
		"cbuffer transform : register(b0)\n"
		"{\n"
		"    float4x4 project;\n"
		"}\n"
		"struct output\n"
		"{\n"
		"	float4 position : SV_POSITION;\n"
		"	float2 texcoord : TEXCOORD;\n"
		"};\n"
		"output main(float2 position : POSITION, float2 texcoord : TEXCOORD)\n"
		"{\n"
		"	output res;\n"
		"	res.position = mul(float4(position, 0, 1), project);\n"
		"   res.texcoord = texcoord;\n"
		"	return res;"
		"}\n";

	static const auto pixelShaderText =
		"float4 main(float4 position : SV_POSITION, float2 texcoord : TEXCOORD) : SV_TARGET\n"
		"{\n"
		"    return float4(1.0f, 1.0f, 1.0f, 1.0f) * (1.01f - length(texcoord));\n"
		"}\n";

	WRL::ComPtr<ID3DBlob> error;
	WRL::ComPtr<ID3DBlob> vertex;
	WRL::ComPtr<ID3DBlob> pixel;

	CODE_RED_THROW_IF_FAILED(
		D3DCompile(vertexShaderText,
			std::strlen(vertexShaderText),
			nullptr, nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE,
			"main", "vs_5_0", D3DCOMPILE_DEBUG, 0,
			vertex.GetAddressOf(), error.GetAddressOf()),
		CodeRed::FailedException({ "Vertex Shader of HLSL" }, CodeRed::DebugType::Create)
	);

	CODE_RED_THROW_IF_FAILED(
		D3DCompile(pixelShaderText,
			std::strlen(pixelShaderText),
			nullptr, nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE,
			"main", "ps_5_0", D3DCOMPILE_DEBUG, 0,
			pixel.GetAddressOf(), error.GetAddressOf()),
		CodeRed::FailedException({ "Pixel Shader of HLSL" }, CodeRed::DebugType::Create)
	);

	mVertexShaderCode = std::vector<CodeRed::Byte>(vertex->GetBufferSize());
	mPixelShaderCode = std::vector<CodeRed::Byte>(pixel->GetBufferSize());
	
	std::memcpy(mVertexShaderCode.data(), vertex->GetBufferPointer(), vertex->GetBufferSize());
	std::memcpy(mPixelShaderCode.data(), pixel->GetBufferPointer(), pixel->GetBufferSize());
#else
#ifdef __VULKAN__MODE
	static const auto vertShaderText =
		"#version 450\n"
		"#extension GL_ARB_separate_shader_objects : enable\n"
		"layout (set = 0, binding = 0) uniform bufferVals {\n"
		"    mat4 project;\n"
		"} myBufferVals;\n"
		"layout (location = 0) in vec4 pos;\n"
		"layout (location = 1) in vec2 tex;\n"
		"layout (location = 1) out vec2 out_tex;\n"
		"void main() {\n"
		"   gl_Position = pos * transpose(myBufferVals.project);\n"
		"	gl_Position.y = - gl_Position.y;\n"
		"	out_tex = tex;\n"
		"}\n";

	static const auto pixelShaderText =
		"#version 450\n"
		"#extension GL_ARB_separate_shader_objects : enable\n"
		"layout (location = 1) in vec2 tex;\n"
		"layout (location = 0) out vec4 outColor;\n"
		"void main() {\n"
		"   outColor = vec4(1.0f, 0.0f, 0.0f, 1.0f) * (1.01f - length(tex));\n"
		"}\n";

	const auto vertex = CodeRed::ShaderCompiler::compileToSpv(CodeRed::ShaderType::Vertex, vertShaderText);
	const auto pixel = CodeRed::ShaderCompiler::compileToSpv(CodeRed::ShaderType::Pixel, pixelShaderText);

	mVertexShaderCode = std::vector<CodeRed::Byte>((vertex.end() - vertex.begin()) * sizeof(uint32_t));
	mPixelShaderCode = std::vector<CodeRed::Byte>((pixel.end() - pixel.begin()) * sizeof(uint32_t));

	std::memcpy(mVertexShaderCode.data(), &vertex.begin()[0], mVertexShaderCode.size());
	std::memcpy(mPixelShaderCode.data(), &pixel.begin()[0], mPixelShaderCode.size());
#endif
#endif
}

void ParticleTextureGenerator::initializeTextures()
{
	//we create the particle texture with detail size
	//we will render some effect to build this texture
	//so the usage and layout is render target
	mTexture = mDevice->createTexture(
		CodeRed::ResourceInfo::Texture2D(
			mDetailSize,
			mDetailSize,
			CodeRed::PixelFormat::RedGreenBlueAlpha8BitUnknown,
			CodeRed::ResourceUsage::RenderTarget,
			CodeRed::ResourceLayout::GeneralRead
		)
	);

	mFrameBuffer = mDevice->createFrameBuffer(
		mTexture,
		nullptr
	);
}

void ParticleTextureGenerator::initializePipeline()
{
	//create pipeline info and pipeline factory
	mPipelineInfo = std::make_shared<CodeRed::PipelineInfo>(mDevice);
	mPipelineFactory = mDevice->createPipelineFactory();

	//we use the ParticleVertex as input format
	//so the layout is "POSITION"(0), "TEXCOORD"(1), "COLOR"(2)
	//and the format of vertex buffer is triangle list
	mPipelineInfo->setInputAssemblyState(
		mPipelineFactory->createInputAssemblyState(
			{
				CodeRed::InputLayoutElement("POSITION", CodeRed::PixelFormat::RedGreen32BitFloat),
				CodeRed::InputLayoutElement("TEXCOORD", CodeRed::PixelFormat::RedGreen32BitFloat)
			},
			CodeRed::PrimitiveTopology::TriangleList
		)
	);

	//we only use view buffer
	//so we the layout of resource only have one element
	mPipelineInfo->setResourceLayout(
		mDevice->createResourceLayout(
			{ CodeRed::ResourceLayoutElement(CodeRed::ResourceType::Buffer, 0, 0) },
			{}
		)
	);

	mPipelineInfo->setVertexShaderState(
		mPipelineFactory->createShaderState(
			CodeRed::ShaderType::Vertex,
			mVertexShaderCode,
			"main"
		)
	);

	//disable depth test
	mPipelineInfo->setDepthStencilState(
		mPipelineFactory->createDetphStencilState(
			false
		)
	);

	mPipelineInfo->setRasterizationState(
		mPipelineFactory->createRasterizationState(
			CodeRed::FrontFace::Clockwise,
			CodeRed::CullMode::None
		)
	);

	mPipelineInfo->setPixelShaderState(
		mPipelineFactory->createShaderState(
			CodeRed::ShaderType::Pixel,
			mPixelShaderCode,
			"main"
		)
	);

	mPipelineInfo->setRenderPass(
		mDevice->createRenderPass(
			CodeRed::Attachment::RenderTarget(mFrameBuffer->renderTarget()->format(),
				CodeRed::ResourceLayout::RenderTarget,
				CodeRed::ResourceLayout::GeneralRead)
		)
	);
	
	//update state to graphics pipeline
	mPipelineInfo->updateState();
}
