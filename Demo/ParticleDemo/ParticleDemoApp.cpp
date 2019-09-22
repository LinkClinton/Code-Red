#include "ParticleDemoApp.hpp"

#include <random>

void ParticleDemoApp::update(float delta)
{
	const auto speed = 5.0f;
	const auto length = speed * delta;

	for (size_t index = 0; index < mParticles.size(); index++) {
		auto& particle = mParticles[index];
		auto& transform = mTransform[index];

		auto oldPosition = particle.Position;
		auto offset = particle.Forward * length;

		particle.reverseIfOut(offset, width(), height());

		transform = glm::translate(transform, glm::vec3(particle.Position - oldPosition, 0.0f));
	}

	const auto buffer = mFrameResources[mCurrentFrameIndex].
		get<CodeRed::GpuBuffer>("Transform");

	const auto memory = buffer->mapMemory();
	std::memcpy(memory, mTransform.data(), buffer->size());
	buffer->unmapMemory();
}

void ParticleDemoApp::render(float delta)
{
	const float color[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	const auto frameBuffer = 
		mFrameResources[mCurrentFrameIndex].get<CodeRed::GpuFrameBuffer>("FrameBuffer");
	const auto transformBuffer =
		mFrameResources[mCurrentFrameIndex].get<CodeRed::GpuBuffer>("Transform");

	mCommandQueue->waitIdle();
	mCommandAllocator->reset();
	
	mCommandList->beginRecoding();

	mCommandList->setGraphicsPipeline(mPipelineInfo->graphicsPipeline());
	mCommandList->setResourceLayout(mPipelineInfo->graphicsPipeline()->layout());

	mCommandList->setFrameBuffer(frameBuffer);
	mCommandList->setViewPort(frameBuffer->fullViewPort());
	mCommandList->setScissorRect(frameBuffer->fullScissorRect());

	mCommandList->setVertexBuffer(mVertexBuffer);
	mCommandList->setIndexBuffer(mIndexBuffer);

	mCommandList->setGraphicsConstantBuffer(0, mViewBuffer);
	mCommandList->setGraphicsConstantBuffer(1, transformBuffer);
	mCommandList->setGraphicsTexture(2, mParticleTextureGenerator->texture());

	mCommandList->layoutTransition(frameBuffer->renderTarget(), 
		CodeRed::ResourceLayout::RenderTarget);
	
	mCommandList->clearRenderTarget(frameBuffer, color);

	mCommandList->drawIndexed(6, particleCount);
	
	mCommandList->layoutTransition(frameBuffer->renderTarget(), 
		CodeRed::ResourceLayout::Present);
	
	mCommandList->endRecoding();

	mCommandQueue->execute({ mCommandList });
	mSwapChain->present();

	mCurrentFrameIndex = (mCurrentFrameIndex + 1) % maxFrameResources;
}

void ParticleDemoApp::initialize()
{
	//find all adapters we can use
	const auto systemInfo = std::make_shared<CodeRed::DirectX12SystemInfo>();
	const auto adapters = systemInfo->selectDisplayAdapter();

	//create device with adapter we choose
	//for this demo, we choose the second adapter
#ifdef __DIRECTX12__MODE__
	mDevice = std::static_pointer_cast<CodeRed::GpuLogicalDevice>(
		std::make_shared<CodeRed::DirectX12LogicalDevice>(adapters[1])
		);
#endif
	
	initializeParticles();
	initializeCommands();
	initializeSwapChain();
	initializeBuffers();
	initializeShaders();
	initializeSamplers();
	initializeTextures();
	initializePipeline();
}

void ParticleDemoApp::finalize()
{
	mCommandQueue->waitIdle();
}

void ParticleDemoApp::initializeParticles()
{
	std::default_random_engine random(0); //(unsigned int)time(0));

	const std::uniform_real_distribution<float> xRange(0.0f, static_cast<float>(width()));
	const std::uniform_real_distribution<float> yRange(0.0f, static_cast<float>(height()));

	const std::uniform_real_distribution<float> forwardRange(-1.0f, 1.0f);
	const std::uniform_real_distribution<float> sizeRange(0.0f, static_cast<float>(maxParticleSize));

	for (size_t index = 0; index < mParticles.size(); index++) {
		auto& particle = mParticles[index];
		auto& transform = mTransform[index];
		
		particle.Position = glm::vec2(xRange(random), yRange(random));
		particle.Forward = glm::normalize(glm::vec2(forwardRange(random), forwardRange(random)));
		particle.Size = glm::vec2(sizeRange(random));

		transform = glm::translate(glm::mat4x4(1), glm::vec3(particle.Position, 0.0f));
		transform = glm::scale(transform, glm::vec3(particle.Size, 1.0f));
	}
}

void ParticleDemoApp::initializeCommands()
{
	//create the command allocator, queue and list
	//command allocator is memory pool for command list
	//command queue is a queue to submit the command list
	//command list is a recoder of draw, copy and so on
	mCommandAllocator = mDevice->createCommandAllocator();
	mCommandQueue = mDevice->createCommandQueue();
	mCommandList = mDevice->createGraphicsCommandList(mCommandAllocator);
}

void ParticleDemoApp::initializeBuffers()
{
	mVertexBuffer = mDevice->createBuffer(
		CodeRed::ResourceInfo::VertexBuffer(
			sizeof(ParticleVertex),
			4,
			CodeRed::MemoryHeap::Default
		)
	);

	mIndexBuffer = mDevice->createBuffer(
		CodeRed::ResourceInfo::IndexBuffer(
			sizeof(unsigned),
			6,
			CodeRed::MemoryHeap::Default
		)
	);

	mViewBuffer = mDevice->createBuffer(
		CodeRed::ResourceInfo::ConstantBuffer(
			sizeof(glm::mat4x4),
			CodeRed::MemoryHeap::Default
		)
	);

	for (auto& frameResource : mFrameResources) {
		frameResource.set(
			"Transform",
			mDevice->createBuffer(
				CodeRed::ResourceInfo::ConstantBuffer(
					sizeof(glm::mat4x4) * particleCount,
					CodeRed::MemoryHeap::Upload
				)
			)
		);
	}

	std::vector<ParticleVertex> vertices(4);
	std::vector<unsigned> indices = {
		0, 1, 2,
		0, 2, 3
	};

	vertices[0] = ParticleVertex(glm::vec2(0.0f, 0.0f), glm::vec2(0.0f, 0.0f));
	vertices[1] = ParticleVertex(glm::vec2(0.0f, 1.0f), glm::vec2(0.0f, 1.0f));
	vertices[2] = ParticleVertex(glm::vec2(1.0f, 1.0f), glm::vec2(1.0f, 1.0f));
	vertices[3] = ParticleVertex(glm::vec2(1.0f, 0.0f), glm::vec2(1.0f, 0.0f));

	const auto view = glm::orthoLH_ZO(0.0f,
		static_cast<float>(width()),
		static_cast<float>(height()),
		0.0f, 0.0f, 1.0f);

	CodeRed::ResourceHelper::updateBuffer(mDevice, mCommandAllocator, mVertexBuffer, vertices.data());
	CodeRed::ResourceHelper::updateBuffer(mDevice, mCommandAllocator, mIndexBuffer, indices.data());
	CodeRed::ResourceHelper::updateBuffer(mDevice, mCommandAllocator, mViewBuffer, &view);
}

void ParticleDemoApp::initializeShaders()
{
#ifdef __DIRECTX12__MODE__
	//in DirectX 12 mode, we use HLSL and compile them with D3DCompile
	static const auto vertexShaderText =
		"#pragma pack_matrix(row_major)\n"
		"cbuffer view : register(b0)\n"
		"{\n"
		"    float4x4 project;\n"
		"}\n"
		"cbuffer transform : register(b1)\n"
		"{\n"
		"	float4x4 world[1000];\n"
		"}\n"
		"struct output\n"
		"{\n"
		"	float4 position : SV_POSITION;\n"
		"	float2 texcoord : TEXCOORD;\n"
		"	uint id : SV_INSTANCEID;\n"
		"};\n"
		"output main(float2 position : POSITION, float2 texcoord : TEXCOORD, uint id : SV_INSTANCEID)\n"
		"{\n"
		"	output res;\n"
		"	res.position = mul(float4(position, 0, 1), world[id]);\n"
		"	res.position = mul(res.position, project);\n"
		"	res.texcoord = texcoord;\n"
		"	res.id = id;\n"
		"	return res;\n"
		"}\n";

	static const auto pixelShaderText =
		"Texture2D particleTexture : register(t0);\n"
		"SamplerState particleSampler : register(s0);\n"
		"float4 main(float4 position : SV_POSITION, float2 texcoord : TEXCOORD, uint id : SV_INSTANCEID) : SV_TARGET\n"
		"{\n"
		"	float alpha = particleTexture.Sample(particleSampler, texcoord).r;\n"
		"	if (alpha <= 0.0f) discard;\n"
		"	return float4(1, 0, 0, 1 * alpha);\n"
		"}\n";

	WRL::ComPtr<ID3DBlob> error;
	WRL::ComPtr<ID3DBlob> vertex;
	WRL::ComPtr<ID3DBlob> pixel;

	CodeRed::throwIfFailed(
		D3DCompile(vertexShaderText,
			std::strlen(vertexShaderText),
			nullptr, nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE,
			"main", "vs_5_0", D3DCOMPILE_DEBUG, 0,
			vertex.GetAddressOf(), error.GetAddressOf()),
		CodeRed::FailedException({ "Vertex Shader of HLSL" }, CodeRed::DebugType::Create)
	);

	CodeRed::throwIfFailed(
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
#endif
}

void ParticleDemoApp::initializeSamplers()
{
	mSampler = mDevice->createSampler(
		CodeRed::SamplerInfo(
			CodeRed::FilterOptions::MinLinearMagLinearMipLinear
		)
	);
}

void ParticleDemoApp::initializeSwapChain()
{
	//create the swap chain
	//if we want to write the back buffer(to window)
	//we need use the queue that create the swap chain to submit the draw commands
	mSwapChain = mDevice->createSwapChain(
		mCommandQueue,
		{ width(), height(), handle() },
		CodeRed::PixelFormat::RedGreenBlueAlpha8BitUnknown,
		maxFrameResources
	);

	for (size_t index = 0; index < maxFrameResources; index++) {
		mFrameResources[index].set(
			"FrameBuffer",
			mDevice->createFrameBuffer(
				mSwapChain->buffer(index),
				nullptr
			)
		);
	}
}

void ParticleDemoApp::initializeTextures()
{
	const size_t particleDetailSize = 100;
	const size_t particleDetailLevel = 400;

	mParticleTextureGenerator = std::make_shared<ParticleTextureGenerator>(
		mDevice,
		mCommandAllocator,
		particleDetailLevel,
		particleDetailSize);

	mParticleTextureGenerator->run();
	mParticleTextureGenerator->waitFinish();
}

void ParticleDemoApp::initializePipeline()
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
			{
				CodeRed::ResourceLayoutElement(CodeRed::ResourceType::Buffer, 0, 0),
				CodeRed::ResourceLayoutElement(CodeRed::ResourceType::Buffer, 1, 0),
				CodeRed::ResourceLayoutElement(CodeRed::ResourceType::Texture, 0,0)
			},
			{ CodeRed::SamplerLayoutElement(mSampler, 0, 0) }
		)
	);

	mPipelineInfo->setVertexShaderState(
		mPipelineFactory->createShaderState(
			mVertexShaderCode
		)
	);

	//disable depth test
	mPipelineInfo->setDepthStencilState(
		mPipelineFactory->createDetphStencilState(
			CodeRed::PixelFormat::Unknown,
			false
		)
	);

	mPipelineInfo->setRasterizationState(
		mPipelineFactory->createRasterizationState(
			mSwapChain->format(),
			CodeRed::FrontFace::Clockwise,
			CodeRed::CullMode::None
		)
	);

	mPipelineInfo->setPixelShaderState(
		mPipelineFactory->createShaderState(
			mPixelShaderCode
		)
	);
	
	mPipelineInfo->setBlendState(
		mPipelineFactory->createBlendState(
			{
				true,
				CodeRed::BlendOperator::Add,
				CodeRed::BlendOperator::Add,
				CodeRed::BlendFactor::InvSrcAlpha,
				CodeRed::BlendFactor::InvSrcAlpha,
				CodeRed::BlendFactor::SrcAlpha,
				CodeRed::BlendFactor::SrcAlpha
			}
		)
	);
	
	//update state to graphics pipeline
	mPipelineInfo->updateState();
}
