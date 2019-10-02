#include "BezierDemoApp.hpp"

BezierDemoApp::BezierDemoApp(
	const std::string& name,
	const size_t width,
	const size_t height) :
#ifdef __ENABLE__DIRECTX12__
	DemoApp(name + "[DirectX12]", width, height)
#else
#ifdef __ENABLE__VULKAN__
	DemoApp(name + "[Vulkan]", width, height)
#else
	DemoApp(name, width, height)
#endif
#endif	
{
	initialize();
}

BezierDemoApp::~BezierDemoApp()
{
	mCommandQueue->waitIdle();
}

void BezierDemoApp::update(float delta)
{
	mFlowersGenerator->update(delta, 
		glm::orthoLH_ZO(0.0f,
		static_cast<float>(width()),
		static_cast<float>(height()),
		0.0f, 0.0f, 1.0f));

	const auto buffer = mFrameResources[mCurrentFrameIndex].get<CodeRed::GpuBuffer>("TransformedPositions");

	const auto memory = buffer->mapMemory();
	std::memcpy(memory, mFlowersGenerator->positions(), buffer->size());
	buffer->unmapMemory();
}

void BezierDemoApp::render(float delta)
{
	//todo
}

void BezierDemoApp::initialize()
{
#ifdef __DIRECTX12__MODE__
	//find all adapters we can use
	const auto systemInfo = std::make_shared<CodeRed::DirectX12SystemInfo>();
	const auto adapters = systemInfo->selectDisplayAdapter();

	//create device with adapter we choose
	//for this demo, we choose the second adapter
	mDevice = std::static_pointer_cast<CodeRed::GpuLogicalDevice>(
		std::make_shared<CodeRed::DirectX12LogicalDevice>(adapters[0])
		);
#else
#ifdef __VULKAN__MODE
	//find all adapters we can use
	const auto systemInfo = std::make_shared<CodeRed::VulkanSystemInfo>();
	const auto adapters = systemInfo->selectDisplayAdapter();

	//create device with adapter we choose
	//for this demo, we choose the second adapter
	mDevice = std::static_pointer_cast<CodeRed::GpuLogicalDevice>(
		std::make_shared<CodeRed::VulkanLogicalDevice>(adapters[0])
		);
#endif
#endif

	initializeCommands();
	initializeSwapChain();
	initializeBuffers();
	initializeShaders();
	initializeSamplers();
	initializeTextures();
	initializePipeline();
	initializeDescriptorHeaps();
}

void BezierDemoApp::initializeFlowers()
{
	mFlowersGenerator = std::make_shared<FlowersGenerator>(
		width(), height(), flowersCount);
}

void BezierDemoApp::initializeCommands()
{
	//create the command allocator, queue and list
	//command allocator is memory pool for command list
	//command queue is a queue to submit the command list
	//command list is a recoder of draw, copy and so on
	mCommandAllocator = mDevice->createCommandAllocator();
	mCommandQueue = mDevice->createCommandQueue();
	mCommandList = mDevice->createGraphicsCommandList(mCommandAllocator);
}

void BezierDemoApp::initializeSwapChain()
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

void BezierDemoApp::initializeBuffers()
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

	for (auto& frameResource : mFrameResources) {
		frameResource.set(
			"TransformedPositions",
			mDevice->createBuffer(
				CodeRed::ResourceInfo::GroupBuffer(
					sizeof(glm::vec4) * 3,
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

	CodeRed::ResourceHelper::updateBuffer(mDevice, mCommandAllocator, mCommandQueue, mVertexBuffer, vertices.data());
	CodeRed::ResourceHelper::updateBuffer(mDevice, mCommandAllocator, mCommandQueue, mIndexBuffer, indices.data());
}

void BezierDemoApp::initializeShaders()
{
	//todo
}

void BezierDemoApp::initializeSamplers()
{
	//empty
}

void BezierDemoApp::initializeTextures()
{
	//empty
}

void BezierDemoApp::initializePipeline()
{
	//create pipeline info and pipeline factory
	mPipelineInfo = std::make_shared<CodeRed::PipelineInfo>(mDevice);
	mPipelineFactory = mDevice->createPipelineFactory();

	//we use the ParticleVertex as input format
	//so the layout is "POSITION"(0)
	//and the format of vertex buffer is triangle list
	mPipelineInfo->setInputAssemblyState(
		mPipelineFactory->createInputAssemblyState(
			{
				CodeRed::InputLayoutElement("POSITION", CodeRed::PixelFormat::RedGreen32BitFloat)
			},
			CodeRed::PrimitiveTopology::TriangleList
		)
	);

	//we only use view buffer
	//so we the layout of resource only have one element
	mPipelineInfo->setResourceLayout(
		mDevice->createResourceLayout(
			{
				CodeRed::ResourceLayoutElement(CodeRed::ResourceType::GroupBuffer, 0),
				CodeRed::ResourceLayoutElement(CodeRed::ResourceType::GroupBuffer, 1)
			},
			{ }
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

	//update state to graphics pipeline
	mPipelineInfo->updateState();
}

void BezierDemoApp::initializeDescriptorHeaps()
{
	for (auto& frameResource : mFrameResources) {
		auto descriptorHeap = mDevice->createDescriptorHeap(
			mPipelineInfo->graphicsPipeline()->layout()
		);

		auto transformedPositions = frameResource.get<CodeRed::GpuBuffer>("TransformedPositions");
		auto colors = frameResource.get<CodeRed::GpuBuffer>("Colors");

		descriptorHeap->bindBuffer(transformedPositions, 0);
		descriptorHeap->bindBuffer(colors, 1);
		
		frameResource.set(
			"DescriptorHeap",
			descriptorHeap
		);
	}
}
