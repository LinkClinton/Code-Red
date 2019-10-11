#include "TriangleDemoApp.hpp"

TriangleDemoApp::TriangleDemoApp(
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

TriangleDemoApp::~TriangleDemoApp()
{
	//if we want to destroy the demo app, device and so on
	//we need wait for command queue to idle
	mCommandQueue->waitIdle();
}

void TriangleDemoApp::update(float delta)
{
	
}

void TriangleDemoApp::render(float delta)
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
	
	mCommandList->setDescriptorHeap(descriptorHeap);

	mCommandList->beginRenderPass(
		mPipelineInfo->renderPass(),
		frameBuffer);

	mCommandList->setConstant32Bits({ 1.0f, 0.0f, 0.0f, 1.0f });

	mCommandList->draw(3);

	mCommandList->endRenderPass();

	mCommandList->endRecording();

	mCommandQueue->execute({ mCommandList });

	mSwapChain->present();

	mCurrentFrameIndex = (mCurrentFrameIndex + 1) % maxFrameResources;
}

void TriangleDemoApp::initialize()
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

	initializeCommands();
	initializeSwapChain();
	initializeBuffers();
	initializeShaders();
	initializeSamplers();
	initializeTextures();
	initializePipeline();
	initializeDescriptorHeaps();
}

void TriangleDemoApp::initializeCommands()
{
	mCommandAllocator = mDevice->createCommandAllocator();
	mCommandQueue = mDevice->createCommandQueue();
	mCommandList = mDevice->createGraphicsCommandList(mCommandAllocator);
}

void TriangleDemoApp::initializeSwapChain()
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

void TriangleDemoApp::initializeBuffers()
{
	mVertexBuffer = mDevice->createBuffer(
		CodeRed::ResourceInfo::VertexBuffer(
			sizeof(glm::vec3),
			3,
			CodeRed::MemoryHeap::Upload
		)
	);

	mViewBuffer = mDevice->createBuffer(
		CodeRed::ResourceInfo::ConstantBuffer(
			sizeof(glm::mat4x4)
		)
	);

	glm::vec3 triangleVertices[] = {
		glm::vec3(0.5f, 0.25f, 0.0f),
		glm::vec3(0.70f, 0.5f, 0.0f),
		glm::vec3(0.30f,0.5f,0.0f)
	};

	for (auto& vertex : triangleVertices)
		vertex = vertex * glm::vec3(width(), height(), 1.0f);

	auto viewMatrix = glm::orthoLH_ZO(0.0f,
		static_cast<float>(width()),
		static_cast<float>(height()), 0.0f, 0.0f, 1.0f);

	const auto triangleMemory = mVertexBuffer->mapMemory();
	const auto viewMemory = mViewBuffer->mapMemory();

	std::memcpy(triangleMemory, triangleVertices, sizeof(triangleVertices));
	std::memcpy(viewMemory, &viewMatrix, sizeof(viewMatrix));

	mVertexBuffer->unmapMemory();
	mViewBuffer->unmapMemory();
}

void TriangleDemoApp::initializeShaders()
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

void TriangleDemoApp::initializeSamplers()
{
	
}

void TriangleDemoApp::initializeTextures()
{
	
}

void TriangleDemoApp::initializePipeline()
{
	mPipelineInfo = std::make_shared<CodeRed::PipelineInfo>(mDevice);
	mPipelineFactory = mDevice->createPipelineFactory();

	mPipelineInfo->setInputAssemblyState(
		mPipelineFactory->createInputAssemblyState(
			{
				CodeRed::InputLayoutElement("POSITION", CodeRed::PixelFormat::RedGreenBlue32BitFloat)
			},
			CodeRed::PrimitiveTopology::TriangleList
		)
	);

	mPipelineInfo->setResourceLayout(
		mDevice->createResourceLayout(
			{
				 CodeRed::ResourceLayoutElement(CodeRed::ResourceType::Buffer, 0, 0)
			},
			{ },
			CodeRed::Constant32Bits(4, 1, 0)
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
		mPipelineFactory->createBlendState()
	);

	mPipelineInfo->setRenderPass(
		mDevice->createRenderPass(
			CodeRed::Attachment::RenderTarget(mSwapChain->format())
		)
	);

	mPipelineInfo->updateState();
}

void TriangleDemoApp::initializeDescriptorHeaps()
{
	for (auto& frameResource : mFrameResources) {
		auto descriptorHeap = mDevice->createDescriptorHeap(
			mPipelineInfo->resourceLayout()
		);

		descriptorHeap->bindBuffer(mViewBuffer, 0);

		frameResource.set(
			"DescriptorHeap",
			descriptorHeap
		);
	}
}

