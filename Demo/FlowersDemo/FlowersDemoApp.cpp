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
	const float color[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	const auto frameBuffer =
		mFrameResources[mCurrentFrameIndex].get<CodeRed::GpuFrameBuffer>("FrameBuffer");
	const auto descriptorHeap =
		mFrameResources[mCurrentFrameIndex].get<CodeRed::GpuDescriptorHeap>("DescriptorHeap");

	mCommandQueue->waitIdle();
	mCommandAllocator->reset();

	mCommandList->beginRecoding();

	mCommandList->setGraphicsPipeline(mPipelineInfo->graphicsPipeline());
	mCommandList->setResourceLayout(mPipelineInfo->graphicsPipeline()->layout());

	mCommandList->setViewPort(frameBuffer->fullViewPort());
	mCommandList->setScissorRect(frameBuffer->fullScissorRect());

	mCommandList->setVertexBuffer(mVertexBuffer);
	mCommandList->setIndexBuffer(mIndexBuffer);

	mCommandList->setDescriptorHeap(descriptorHeap);

	mCommandList->beginRenderPass(
		mPipelineInfo->graphicsPipeline()->renderPass(),
		frameBuffer);

	mCommandList->drawIndexed(3, flowersCount * 8);

	mCommandList->endRenderPass();

	mCommandList->endRecoding();

	mCommandQueue->execute({ mCommandList });

	mSwapChain->present();

	mCurrentFrameIndex = (mCurrentFrameIndex + 1) % maxFrameResources;
}

void FlowersDemoApp::initialize()
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
#ifdef __VULKAN__MODE__
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
	//create the command allocator, queue and list
	//command allocator is memory pool for command list
	//command queue is a queue to submit the command list
	//command list is a recoder of draw, copy and so on
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
	const auto vertexShaderText = CodeRed::ShaderCompiler::readShader("./Shaders/DirectX12VertexShader.hlsl");
	const auto pixelShaderText = CodeRed::ShaderCompiler::readShader("./Shaders/DirectX12PixelShader.hlsl");
	
	WRL::ComPtr<ID3DBlob> error;
	WRL::ComPtr<ID3DBlob> vertex;
	WRL::ComPtr<ID3DBlob> pixel;

#ifdef _DEBUG
	const UINT flag = D3DCOMPILE_DEBUG;
#else
	const UINT flag = 0;
#endif
	
	CODE_RED_THROW_IF_FAILED(
		D3DCompile(vertexShaderText.data(),
			vertexShaderText.length(),
			nullptr, nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE,
			"main", "vs_5_1", flag, 0,
			vertex.GetAddressOf(), error.GetAddressOf()),
		CodeRed::FailedException(CodeRed::DebugType::Create, { "Vertex Shader of HLSL" },
			{ CodeRed::DirectX12::charArrayToString(error->GetBufferPointer(), error->GetBufferSize()) })
	);

	CODE_RED_THROW_IF_FAILED(
		D3DCompile(pixelShaderText.data(),
			pixelShaderText.length(),
			nullptr, nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE,
			"main", "ps_5_1", flag, 0,
			pixel.GetAddressOf(), error.GetAddressOf()),
		CodeRed::FailedException(CodeRed::DebugType::Create, { "Pixel Shader of HLSL" },
			{ CodeRed::DirectX12::charArrayToString(error->GetBufferPointer(), error->GetBufferSize()) })
	);

	mVertexShaderCode = std::vector<CodeRed::Byte>(vertex->GetBufferSize());
	mPixelShaderCode = std::vector<CodeRed::Byte>(pixel->GetBufferSize());

	std::memcpy(mVertexShaderCode.data(), vertex->GetBufferPointer(), vertex->GetBufferSize());
	std::memcpy(mPixelShaderCode.data(), pixel->GetBufferPointer(), pixel->GetBufferSize());
#else
#ifdef __VULKAN__MODE__
#endif
	const auto vertexShaderText = CodeRed::ShaderCompiler::readShader("./Shaders/VulkanVertex.vert");
	const auto pixelShaderText = CodeRed::ShaderCompiler::readShader("./Shaders/VulkanFragment.frag");

	const auto vertex = CodeRed::ShaderCompiler::compileToSpv(CodeRed::ShaderType::Vertex, vertexShaderText);
	const auto pixel = CodeRed::ShaderCompiler::compileToSpv(CodeRed::ShaderType::Pixel, pixelShaderText);

	mVertexShaderCode = std::vector<CodeRed::Byte>((vertex.end() - vertex.begin()) * sizeof(uint32_t));
	mPixelShaderCode = std::vector<CodeRed::Byte>((pixel.end() - pixel.begin()) * sizeof(uint32_t));

	std::memcpy(mVertexShaderCode.data(), &vertex.begin()[0], mVertexShaderCode.size());
	std::memcpy(mPixelShaderCode.data(), &pixel.begin()[0], mPixelShaderCode.size());
#endif
}

void FlowersDemoApp::initializeSamplers()
{
	//empty
}

void FlowersDemoApp::initializeTextures()
{
	//empty
}

void FlowersDemoApp::initializePipeline()
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
				CodeRed::ResourceLayoutElement(CodeRed::ResourceType::GroupBuffer, 1),
				CodeRed::ResourceLayoutElement(CodeRed::ResourceType::Buffer, 2)
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

void FlowersDemoApp::initializeDescriptorHeaps()
{
	for (auto& frameResource : mFrameResources) {
		auto descriptorHeap = mDevice->createDescriptorHeap(
			mPipelineInfo->graphicsPipeline()->layout()
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
