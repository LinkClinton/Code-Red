#include "FlowersDemoApp.hpp"

FlowersDemoApp::FlowersDemoApp(
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

FlowersDemoApp::~FlowersDemoApp()
{
	//if we want to destroy the demo app, device and so on
	//we need wait for command queue to idle
	mCommandQueue->waitIdle();
}

void FlowersDemoApp::update(float delta)
{
	mFlowersGenerator->update(delta);

	const auto buffer = mFrameResources[mCurrentFrameIndex].get<CodeRed::GpuBuffer>("TransformedPositions");

	const auto memory = buffer->mapMemory();
	std::memcpy(memory, mFlowersGenerator->positions(), buffer->size());
	buffer->unmapMemory();
}

void FlowersDemoApp::render(float delta)
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

	mCommandList->drawIndexed(3, flowersCount * 8);

	mCommandList->endRenderPass();

	mCommandList->endRecording();

	mCommandQueue->execute({ mCommandList });

	mSwapChain->present();

	mCurrentFrameIndex = (mCurrentFrameIndex + 1) % maxFrameResources;
}

void FlowersDemoApp::initialize()
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

	initializeFlowers();
	initializeCommands();
	initializeSwapChain();
	initializeBuffers();
	initializeShaders();
	initializeSamplers();
	initializeTextures();
	initializePipeline();
	initializeDescriptorHeaps();
}

void FlowersDemoApp::initializeFlowers()
{
	mFlowersGenerator = std::make_shared<FlowersGenerator>(
		width(), height(), flowersCount);
}

void FlowersDemoApp::initializeCommands()
{
	mCommandAllocator = mDevice->createCommandAllocator();
	mCommandQueue = mDevice->createCommandQueue();
	mCommandList = mDevice->createGraphicsCommandList(mCommandAllocator);
}

void FlowersDemoApp::initializeSwapChain()
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

void FlowersDemoApp::initializeBuffers()
{
	mVertexBuffer = mDevice->createBuffer(
		CodeRed::ResourceInfo::VertexBuffer(
			sizeof(glm::vec2),
			3,
			CodeRed::MemoryHeap::Default
		)
	);

	mIndexBuffer = mDevice->createBuffer(
		CodeRed::ResourceInfo::IndexBuffer(
			sizeof(unsigned),
			3,
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
			"TransformedPositions",
			mDevice->createBuffer(
				CodeRed::ResourceInfo::GroupBuffer(
					sizeof(glm::vec2) * 3,
					flowersCount * 8
				)
			)
		);

		frameResource.set(
			"Colors",
			mDevice->createBuffer(
				CodeRed::ResourceInfo::GroupBuffer(
					sizeof(glm::vec4) * 3,
					flowersCount * 8,
					CodeRed::MemoryHeap::Default
				)
			)
		);

		auto colors = frameResource.get<CodeRed::GpuBuffer>("Colors");

		CodeRed::ResourceHelper::updateBuffer(mDevice, mCommandAllocator, mCommandQueue, colors, mFlowersGenerator->colors());
	}

	std::vector<glm::vec2> vertices = {
		glm::vec2(0, 0),
		glm::vec2(0.5f, 0),
		glm::vec2(1, 1)
	};

	std::vector<unsigned> indices = { 0, 1, 2 };

	const auto view = glm::orthoLH_ZO(0.0f,
		static_cast<float>(width()),
		static_cast<float>(height()),
		0.0f, 0.0f, 1.0f);
	
	CodeRed::ResourceHelper::updateBuffer(mDevice, mCommandAllocator, mCommandQueue, mVertexBuffer, vertices.data());
	CodeRed::ResourceHelper::updateBuffer(mDevice, mCommandAllocator, mCommandQueue, mIndexBuffer, indices.data());
	CodeRed::ResourceHelper::updateBuffer(mDevice, mCommandAllocator, mCommandQueue, mViewBuffer, &view);
}

void FlowersDemoApp::initializeShaders()
{
#ifdef __DIRECTX12__MODE__
	const auto vertexShaderText = CodeRed::ShaderCompiler::readShader("./Shaders/DirectX12Vertex.hlsl");
	const auto pixelShaderText = CodeRed::ShaderCompiler::readShader("./Shaders/DirectX12Pixel.hlsl");

	mVertexShaderCode = CodeRed::ShaderCompiler::compileToCso(CodeRed::ShaderType::Vertex, vertexShaderText);
	mPixelShaderCode = CodeRed::ShaderCompiler::compileToCso(CodeRed::ShaderType::Pixel, pixelShaderText);
#else
#ifdef __VULKAN__MODE__
#endif
	const auto vertexShaderText = CodeRed::ShaderCompiler::readShader("./Shaders/VulkanVertex.vert");
	const auto pixelShaderText = CodeRed::ShaderCompiler::readShader("./Shaders/VulkanFragment.frag");

	mVertexShaderCode = CodeRed::ShaderCompiler::compileToSpv(CodeRed::ShaderType::Vertex, vertexShaderText);
	mPixelShaderCode = CodeRed::ShaderCompiler::compileToSpv(CodeRed::ShaderType::Pixel, pixelShaderText);
#endif
}

void FlowersDemoApp::initializeSamplers()
{
	
}

void FlowersDemoApp::initializeTextures()
{
	
}

void FlowersDemoApp::initializePipeline()
{
	mPipelineInfo = std::make_shared<CodeRed::PipelineInfo>(mDevice);
	mPipelineFactory = mDevice->createPipelineFactory();

	mPipelineInfo->setInputAssemblyState(
		mPipelineFactory->createInputAssemblyState(
			{
				CodeRed::InputLayoutElement("POSITION", CodeRed::PixelFormat::RedGreen32BitFloat)
			},
			CodeRed::PrimitiveTopology::TriangleList
		)
	);

	mPipelineInfo->setResourceLayout(
		mDevice->createResourceLayout(
			{
				CodeRed::ResourceLayoutElement(CodeRed::ResourceType::GroupBuffer, 0),
				CodeRed::ResourceLayoutElement(CodeRed::ResourceType::GroupBuffer, 1),
				CodeRed::ResourceLayoutElement(CodeRed::ResourceType::Buffer, 2)
			},
			{ }
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

void FlowersDemoApp::initializeDescriptorHeaps()
{
	for (auto& frameResource : mFrameResources) {
		auto descriptorHeap = mDevice->createDescriptorHeap(
			mPipelineInfo->resourceLayout()
		);

		auto transformedPositions = frameResource.get<CodeRed::GpuBuffer>("TransformedPositions");
		auto colors = frameResource.get<CodeRed::GpuBuffer>("Colors");

		descriptorHeap->bindBuffer(transformedPositions, 0);
		descriptorHeap->bindBuffer(colors, 1);
		descriptorHeap->bindBuffer(mViewBuffer, 2);
		
		frameResource.set(
			"DescriptorHeap",
			descriptorHeap
		);
	}
}
