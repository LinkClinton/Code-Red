#include "ParticlesDemoApp.hpp"

#include <iostream>
#include <random>

void Particle::reverseIfOut(glm::vec2& offset, const size_t width, const size_t height)
{
	const auto target = Position + offset;

	if (target.x - Size.x < 0 || target.x + Size.x > width) {
		offset.x = -offset.x;
		Forward.x = -Forward.x;
	}

	if (target.y - Size.y < 0 || target.y + Size.y > height) {
		offset.y = -offset.y;
		Forward.y = -Forward.y;
	}

	Position = Position + offset;
}

ParticlesDemoApp::ParticlesDemoApp(
	const std::string& name,
	const size_t width,
	const size_t height) :
#ifdef __DIRECTX12__MODE__
	DemoApp(name + "[DirectX12]", width, height)
#else
#ifdef __VULKAN__MODE__
	DemoApp(name + "[Vulkan]", width, height)
#else
	DemoApp(name, width, height)
#endif
#endif	
{
	initialize();
}

ParticlesDemoApp::~ParticlesDemoApp()
{
	//if we want to destroy the demo app, device and so on
	//we need wait for command queue to idle
	mCommandQueue->waitIdle();
}

void ParticlesDemoApp::update(float delta)
{
	const auto speed = 100.0f;
	const auto length = speed * delta;

	for (size_t index = 0; index < mParticles.size(); index++) {
		auto& particle = mParticles[index];
		auto& transform = mTransform[index];
		auto offset = particle.Forward * length;

		particle.reverseIfOut(offset, width(), height());

		transform = glm::translate(glm::mat4x4(1), glm::vec3(offset, 0.0f)) * transform;
	}

	const auto buffer = mFrameResources[mCurrentFrameIndex].get<CodeRed::GpuBuffer>("Transform");

	const auto memory = buffer->mapMemory();
	std::memcpy(memory, mTransform.data(), buffer->size());
	buffer->unmapMemory();
}


void ParticlesDemoApp::render(float delta)
{
	const auto frameBuffer = 
		mFrameResources[mCurrentFrameIndex].get<CodeRed::GpuFrameBuffer>("FrameBuffer");
	const auto descriptorHeap =
		mFrameResources[mCurrentFrameIndex].get<CodeRed::GpuDescriptorHeap>("DescriptorHeap");

	mCommandQueue->waitIdle();
	mCommandAllocator->reset();
	
	mCommandList->beginRecording();

	mCommandList->setGraphicsPipeline(mPipelineInfo->graphicsPipeline());
	mCommandList->setResourceLayout(mPipelineInfo->resourceLayout());

	mCommandList->setViewPort(frameBuffer->fullViewPort());
	mCommandList->setScissorRect(frameBuffer->fullScissorRect());

	mCommandList->setVertexBuffer(mVertexBuffer);
	mCommandList->setIndexBuffer(mIndexBuffer);

	mCommandList->setDescriptorHeap(descriptorHeap);
	
	mCommandList->beginRenderPass(
		mPipelineInfo->renderPass(),
		frameBuffer);
	
	mCommandList->drawIndexed(6, particleCount);
	
	mCommandList->endRenderPass();
		
	mCommandList->endRecording();

	mCommandQueue->execute({ mCommandList });

	mSwapChain->present();

	mCurrentFrameIndex = (mCurrentFrameIndex + 1) % maxFrameResources;
}

void ParticlesDemoApp::initialize()
{

#ifdef __DIRECTX12__MODE__
	const auto systemInfo = std::make_shared<CodeRed::DirectX12SystemInfo>();
	const auto adapters = systemInfo->selectDisplayAdapter();

	mDevice = std::static_pointer_cast<CodeRed::GpuLogicalDevice>(
		std::make_shared<CodeRed::DirectX12LogicalDevice>(adapters[0])
		);
#else
#ifdef __VULKAN__MODE__
	const auto systemInfo = std::make_shared<CodeRed::VulkanSystemInfo>();
	const auto adapters = systemInfo->selectDisplayAdapter();
	
	mDevice = std::static_pointer_cast<CodeRed::GpuLogicalDevice>(
		std::make_shared<CodeRed::VulkanLogicalDevice>(adapters[0])
		);
#endif
#endif
	
	initializeParticles();
	initializeCommands();
	initializeSwapChain();
	initializeBuffers();
	initializeShaders();
	initializeSamplers();
	initializeTextures();
	initializePipeline();
	initializeDescriptorHeaps();
}

void ParticlesDemoApp::initializeParticles()
{
	std::default_random_engine random(0);

	const std::uniform_real_distribution<float> xRange(0.0f, static_cast<float>(width()));
	const std::uniform_real_distribution<float> yRange(0.0f, static_cast<float>(height()));

	const std::uniform_real_distribution<float> forwardRange(-1.0f, 1.0f);
	const std::uniform_real_distribution<float> sizeRange(10.0f, static_cast<float>(maxParticleSize));

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

void ParticlesDemoApp::initializeCommands()
{
	mCommandAllocator = mDevice->createCommandAllocator();
	mCommandQueue = mDevice->createCommandQueue();
	mCommandList = mDevice->createGraphicsCommandList(mCommandAllocator);
}

void ParticlesDemoApp::initializeBuffers()
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
		auto buffer = mDevice->createBuffer(
			CodeRed::ResourceInfo::GroupBuffer(
				sizeof(glm::mat4x4),
				particleCount
			)
		);
		
		frameResource.set(
			"Transform",
			buffer
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

void ParticlesDemoApp::initializeShaders()
{
#ifdef __DIRECTX12__MODE__
	const auto vertexShaderText = CodeRed::ShaderCompiler::readShader("./Shaders/DirectX12Vertex.hlsl");
	const auto pixelShaderText = CodeRed::ShaderCompiler::readShader("./Shaders/DirectX12Pixel.hlsl");

	mVertexShaderCode = CodeRed::ShaderCompiler::compileToCso(CodeRed::ShaderType::Vertex, vertexShaderText);
	mPixelShaderCode = CodeRed::ShaderCompiler::compileToCso(CodeRed::ShaderType::Pixel, pixelShaderText);
#else
#ifdef __VULKAN__MODE__
	const auto vertexShaderText = CodeRed::ShaderCompiler::readShader("./Shaders/VulkanVertex.vert");
	const auto pixelShaderText = CodeRed::ShaderCompiler::readShader("./Shaders/VulkanFragment.frag");

	mVertexShaderCode = CodeRed::ShaderCompiler::compileToSpv(CodeRed::ShaderType::Vertex, vertexShaderText);
	mPixelShaderCode = CodeRed::ShaderCompiler::compileToSpv(CodeRed::ShaderType::Pixel, pixelShaderText);
#endif
#endif
}

void ParticlesDemoApp::initializeSamplers()
{
	mSampler = mDevice->createSampler(
		CodeRed::SamplerInfo(
			CodeRed::FilterOptions::MinLinearMagLinearMipLinear
		)
	);
}

void ParticlesDemoApp::initializeSwapChain()
{
	//create the swap chain
	//if we want to write the back buffer(to window)
	//we need use the queue that create the swap chain to submit the draw commands
	mSwapChain = mDevice->createSwapChain(
		mCommandQueue,
		{ width(), height(), handle() },
		CodeRed::PixelFormat::BlueGreenRedAlpha8BitUnknown,
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

void ParticlesDemoApp::initializeTextures()
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

void ParticlesDemoApp::initializePipeline()
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
			{
				CodeRed::ResourceLayoutElement(CodeRed::ResourceType::Buffer, 0, 0),
				CodeRed::ResourceLayoutElement(CodeRed::ResourceType::GroupBuffer, 1, 0),
				CodeRed::ResourceLayoutElement(CodeRed::ResourceType::Texture, 2,0)
			},
			{ CodeRed::SamplerLayoutElement(mSampler, 3, 0) }
		)
	);

	//the name only enabled for vulkan version
	//this is the entry of shader in glsl
	//the entry of hlsl is bound when we compile it
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

	//the name only enabled for vulkan version
	//this is the entry of shader in glsl
	//the entry of hlsl is bound when we compile it
	mPipelineInfo->setPixelShaderState(
		mPipelineFactory->createShaderState(
			CodeRed::ShaderType::Pixel,
			mPixelShaderCode,
			"main"
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

	mPipelineInfo->setRenderPass(
		mDevice->createRenderPass(
			CodeRed::Attachment::RenderTarget(mSwapChain->format())
		)
	);
	
	mPipelineInfo->updateState();
}

void ParticlesDemoApp::initializeDescriptorHeaps()
{
	for (auto& frameResource : mFrameResources) {
		auto descriptorHeap = mDevice->createDescriptorHeap(
			mPipelineInfo->resourceLayout()
		);

		auto buffer = frameResource.get<CodeRed::GpuBuffer>("Transform");

		descriptorHeap->bindBuffer(mViewBuffer, 0);
		descriptorHeap->bindBuffer(buffer, 1);
		descriptorHeap->bindTexture(mParticleTextureGenerator->texture(), 2);

		frameResource.set(
			"DescriptorHeap",
			descriptorHeap
		);
	}
}
