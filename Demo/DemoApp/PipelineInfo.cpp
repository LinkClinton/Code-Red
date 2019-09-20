#include "PipelineInfo.hpp"

CodeRed::PipelineInfo::PipelineInfo(const std::shared_ptr<GpuLogicalDevice>& device)
	: mDevice(device)
{
	mPipelineFactory = mDevice->createPipelineFactory();

	mRasterizationState = mPipelineFactory->createRasterizationState(PixelFormat::Unknown);
	mInputAssemblyState = mPipelineFactory->createInputAssemblyState({});
	mVertexShaderState = mPipelineFactory->createShaderState({});
	mDepthStencilState = mPipelineFactory->createDetphStencilState(PixelFormat::Unknown);
	mPixelShaderState = mPipelineFactory->createShaderState({});
	mBlendState = mPipelineFactory->createBlendState();
	mResourceLayout = mDevice->createResourceLayout({}, {});
}

void CodeRed::PipelineInfo::setRasterizationState(const std::shared_ptr<GpuRasterizationState>& state)
{
	mRasterizationState = state;
}

void CodeRed::PipelineInfo::setInputAssemblyState(const std::shared_ptr<GpuInputAssemblyState>& state)
{
	mInputAssemblyState = state;
}

void CodeRed::PipelineInfo::setVertexShaderState(const std::shared_ptr<GpuShaderState>& state)
{
	mVertexShaderState = state;
}

void CodeRed::PipelineInfo::setDepthStencilState(const std::shared_ptr<GpuDepthStencilState>& state)
{
	mDepthStencilState = state;
}

void CodeRed::PipelineInfo::setPixelShaderState(const std::shared_ptr<GpuShaderState>& state)
{
	mPixelShaderState = state;
}

void CodeRed::PipelineInfo::setResourceLayout(const std::shared_ptr<GpuResourceLayout>& layout)
{
	mResourceLayout = layout;
}

void CodeRed::PipelineInfo::setBlendState(const std::shared_ptr<GpuBlendState>& state)
{
	mBlendState = state;
}

void CodeRed::PipelineInfo::updateState()
{
	mGraphicsPipeline = mDevice->createGraphicsPipeline(
		mResourceLayout,
		mInputAssemblyState,
		mVertexShaderState,
		mPixelShaderState,
		mDepthStencilState,
		mBlendState,
		mRasterizationState
	);
}
