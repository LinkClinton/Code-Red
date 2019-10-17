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
	initializeDescriptorHeaps();
}

void ParticleTextureGenerator::run() const
{
	//the color we used to clear the render target
	//because we want to build a particle texture
	//the pixel not in the circle is (0, 0, 0, 0)
	//we will use blend or clip to ignore this pixel
	auto renderPass = mPipelineInfo->renderPass();

	renderPass->setClear(CodeRed::ClearValue(0.0f, 0.0f, 0.0f, 0.0f));

	mCommandList->beginRecording();

	mCommandList->setGraphicsPipeline(mPipelineInfo->graphicsPipeline());
	mCommandList->setResourceLayout(mPipelineInfo->resourceLayout());
	
	mCommandList->setViewPort(mFrameBuffer->fullViewPort());
	mCommandList->setScissorRect(mFrameBuffer->fullScissorRect());

	mCommandList->setVertexBuffer(mVertexBuffer);
	mCommandList->setIndexBuffer(mIndexBuffer);
	mCommandList->setDescriptorHeap(mDescriptorHeap);
	
	mCommandList->beginRenderPass(renderPass, mFrameBuffer);
	
	mCommandList->drawIndexed(mIndexBuffer->count());

	mCommandList->endRenderPass();
	
	mCommandList->endRecording();

	mCommandQueue->execute({ mCommandList });
}

void ParticleTextureGenerator::waitFinish() const
{
	mCommandQueue->waitIdle();
}

void ParticleTextureGenerator::initializeCommands()
{
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
	const auto vertexShaderText = CodeRed::ShaderCompiler::readShader("./Shaders/DirectX12TextureVertex.hlsl");
	const auto pixelShaderText = CodeRed::ShaderCompiler::readShader("./Shaders/DirectX12TexturePixel.hlsl");

	mVertexShaderCode = CodeRed::ShaderCompiler::compileToCso(CodeRed::ShaderType::Vertex, vertexShaderText);
	mPixelShaderCode = CodeRed::ShaderCompiler::compileToCso(CodeRed::ShaderType::Pixel, pixelShaderText);
#else
#ifdef __VULKAN__MODE__
	const auto vertexShaderText = CodeRed::ShaderCompiler::readShader("./Shaders/VulkanTextureVertex.vert");
	const auto pixelShaderText = CodeRed::ShaderCompiler::readShader("./Shaders/VulkanTextureFragment.frag");

	mVertexShaderCode = CodeRed::ShaderCompiler::compileToSpv(CodeRed::ShaderType::Vertex, vertexShaderText);
	mPixelShaderCode = CodeRed::ShaderCompiler::compileToSpv(CodeRed::ShaderType::Pixel, pixelShaderText);
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
			CodeRed::ResourceUsage::RenderTarget
		)
	);

	mFrameBuffer = mDevice->createFrameBuffer(
		mTexture,
		nullptr
	);
}

void ParticleTextureGenerator::initializePipeline()
{
	mPipelineInfo = std::make_shared<CodeRed::PipelineInfo>(mDevice);
	mPipelineFactory = mDevice->createPipelineFactory();

	mPipelineInfo->setInputAssemblyState(
		mPipelineFactory->createInputAssemblyState(
			{
				CodeRed::InputLayoutElement("POSITION", CodeRed::PixelFormat::RedGreen32BitFloat),
				CodeRed::InputLayoutElement("TEXCOORD", CodeRed::PixelFormat::RedGreen32BitFloat)
			},
			CodeRed::PrimitiveTopology::TriangleList
		)
	);

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
	
	mPipelineInfo->updateState();
}

void ParticleTextureGenerator::initializeDescriptorHeaps()
{
	mDescriptorHeap = mDevice->createDescriptorHeap(
		mPipelineInfo->resourceLayout()
	);

	mDescriptorHeap->bindBuffer(mViewBuffer, 0);
}
