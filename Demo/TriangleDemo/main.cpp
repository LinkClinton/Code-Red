#include <DemoApp.hpp>

#define __DIRECTX12__MODE__

class TriangleDemo final : public Demo::DemoApp {
public:
	TriangleDemo(
		const std::string &name, 
		const size_t width, 
		const size_t height) :
		Demo::DemoApp(name, width, height)
	{
		initialize();
	}
private:
	void update() override {}
	void render() override
	{
		const auto index = mSwapChain->currentBufferIndex();
		const float color[] = { 1.0f, 1.0f, 1.0f, 1.0f };
		
		mCommandAllocator->reset();
		mCommandList->reset();

		mCommandList->beginRecoding();
		mCommandList->layoutTransition(mFrameBuffers[index]->renderTarget(), CodeRed::ResourceLayout::RenderTarget);
		mCommandList->clearRenderTarget(mFrameBuffers[index], color);
		mCommandList->layoutTransition(mFrameBuffers[index]->renderTarget(), CodeRed::ResourceLayout::Present);
		mCommandList->endRecoding();

		mCommandQueue->execute({ mCommandList });

		mSwapChain->present();
		
		mCommandQueue->waitIdle();

	}
	void initialize() override
	{
		//find all adapters we can use
		const auto systemInfo = std::make_shared<CodeRed::DirectX12SystemInfo>();
		const auto adapters = systemInfo->selectDisplayAdapter();

		//create device with adapter we choose
		//for this demo, we choose the first adapter
#ifdef __DIRECTX12__MODE__
		mDevice = std::static_pointer_cast<CodeRed::GpuLogicalDevice>(
			std::make_shared<CodeRed::DirectX12LogicalDevice>(adapters[0])
			);
#endif

		mCommandAllocator = mDevice->createCommandAllocator();
		mCommandQueue = mDevice->createCommandQueue();
		mCommandList = mDevice->createGraphicsCommandList(mCommandAllocator);
		
		mSwapChain = mDevice->createSwapChain(
			mCommandQueue,
			{ width(), height(), handle() },
			CodeRed::PixelFormat::RedGreenBlueAlpha8BitUnknown
		);

		for (size_t index = 0; index < mSwapChain->bufferCount(); index++) {

			const auto frameBuffer = mDevice->createFrameBuffer(
				mSwapChain->buffer(index),
				nullptr);

			mFrameBuffers.push_back(frameBuffer);
		}

	}
private:
	std::shared_ptr<CodeRed::GpuLogicalDevice> mDevice;
	std::shared_ptr<CodeRed::GpuSwapChain> mSwapChain;
	
	std::shared_ptr<CodeRed::GpuCommandAllocator> mCommandAllocator;
	std::shared_ptr<CodeRed::GpuGraphicsCommandList> mCommandList;
	std::shared_ptr<CodeRed::GpuCommandQueue> mCommandQueue;
	
	std::vector<std::shared_ptr<CodeRed::GpuFrameBuffer>> mFrameBuffers;
};

int main() {
	auto app = TriangleDemo("TriangleDemo", 1920, 1080);
	
	app.show();
	app.runLoop();
}