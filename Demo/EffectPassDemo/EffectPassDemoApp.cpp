#include "EffectPassDemoApp.hpp"

EffectPassDemoApp::EffectPassDemoApp(
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

EffectPassDemoApp::~EffectPassDemoApp()
{
	//if we want to destroy the demo app, device and so on
	//we need wait for command queue to idle
	mCommandQueue->waitIdle();
}

void EffectPassDemoApp::update(float delta)
{
	//user can update some resource or state in this function
	//such as, update buffer, copy texture and so on
	//the function call is before render()
	//todo
}

void EffectPassDemoApp::render(float delta)
{
	//get the frame buffer we render to
	//we use a frame resource to record the resource we will use in this frame
	//with frame resource, we can try to avoid the synchronization of CPU and GPU
	const auto frameBuffer =
		mFrameResources[mCurrentFrameIndex].get<CodeRed::GpuFrameBuffer>("FrameBuffer");
	const auto effectPass =
		mFrameResources[mCurrentFrameIndex].get<CodeRed::EffectPass>("EffectPass");

	//wait for GPU and reset the command allocator
	mCommandQueue->waitIdle();
	mCommandAllocator->reset();

	//begin to recording commands
	mCommandList->beginRecording();

	//set view port and scissor rect
	mCommandList->setViewPort(frameBuffer->fullViewPort());
	mCommandList->setScissorRect(frameBuffer->fullScissorRect());

	//set vertex buffer and index buffer
	//todo

	effectPass->beginEffect(mCommandList);
	
	//begin render pass and set frame buffer
	mCommandList->beginRenderPass(
		mRenderPass,
		frameBuffer);

	//add some draw commands
	//the draw commands must between the render pass
	//todo
	
	mCommandList->endRenderPass();

	effectPass->endEffect();
	
	mCommandList->endRecording();

	//execute the commands recording by command list
	mCommandQueue->execute({ mCommandList });

	mSwapChain->present();

	mCurrentFrameIndex = (mCurrentFrameIndex + 1) % maxFrameResources;
}

void EffectPassDemoApp::initialize()
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

void EffectPassDemoApp::initializeCommands()
{
	//create command allocator, queue and list
	//command allocator is a memory pool to allocator memory of command list
	//the commands recoding by command list are store in the command allocator
	//the command queue is a queue that store the commands are submitted to GPU
	mCommandAllocator = mDevice->createCommandAllocator();
	mCommandQueue = mDevice->createCommandQueue();
	mCommandList = mDevice->createGraphicsCommandList(mCommandAllocator);
}

void EffectPassDemoApp::initializeSwapChain()
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

void EffectPassDemoApp::initializeBuffers()
{
	//we initialize buffers in this
	//such as vertex buffer, index buffer, constant buffer
	//the buffers in the frame resource are created in this, too.
}

void EffectPassDemoApp::initializeShaders()
{
	//we initialize shaders in this
	//we will read shader from file and compile them
	//there are some help function in class CodeRed::ShaderCompiler in DemoApp.
#ifdef __DIRECTX12__MODE__
#else
#ifdef __VULKAN__MODE__
#endif
#endif
}

void EffectPassDemoApp::initializeSamplers()
{
	//we initialize samplers in this
}

void EffectPassDemoApp::initializeTextures()
{
	//we initialize textures in this
}

void EffectPassDemoApp::initializePipeline()
{
	//we initialize the graphics pipeline in this
	//the pipeline info is a help class that we can initialize pipeline easier
	//you can use setXXX to set the graphics pipeline state
	//but you should use "updateState()" to create graphics pipeline
	mRenderPass = mDevice->createRenderPass(
		CodeRed::Attachment::RenderTarget(mSwapChain->format())
	);

	for (auto& frameResource : mFrameResources) {
		frameResource.set("EffectPass",
			std::make_shared<CodeRed::EffectPass>(
				mDevice,
				mRenderPass)
		);
	}
}

void EffectPassDemoApp::initializeDescriptorHeaps()
{
	//we initialize descriptor heap in this
	
}

