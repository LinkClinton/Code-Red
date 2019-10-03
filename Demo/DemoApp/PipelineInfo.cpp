#include "PipelineInfo.hpp"

CodeRed::PipelineInfo::PipelineInfo(const std::shared_ptr<GpuLogicalDevice>& device)
	: mDevice(device)
{
	mPipelineFactory = mDevice->createPipelineFactory();

	mRasterizationState = mPipelineFactory->createRasterizationState();
	mInputAssemblyState = mPipelineFactory->createInputAssemblyState({});
	mDepthStencilState = mPipelineFactory->createDetphStencilState();
	mBlendState = mPipelineFactory->createBlendState();
	mResourceLayout = mDevice->createResourceLayout({}, {});
	mRenderPass = mDevice->createRenderPass(Attachment::RenderTarget(PixelFormat::BlueGreenRedAlpha8BitUnknown));
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

void CodeRed::PipelineInfo::setRenderPass(const std::shared_ptr<GpuRenderPass>& render_pass) {
	mRenderPass = render_pass;
}

void CodeRed::PipelineInfo::updateState()
{
	mGraphicsPipeline = mDevice->createGraphicsPipeline(
		mRenderPass,
		mResourceLayout,
		mInputAssemblyState,
		mVertexShaderState,
		mPixelShaderState,
		mDepthStencilState,
		mBlendState,
		mRasterizationState
	);
}

auto CodeRed::PipelineInfo::rasterizationState() const noexcept
	-> std::shared_ptr<GpuRasterizationState>
{
	return mRasterizationState;
}

auto CodeRed::PipelineInfo::inputAssemblyState() const noexcept
	-> std::shared_ptr<GpuInputAssemblyState>
{
	return mInputAssemblyState;
}

auto CodeRed::PipelineInfo::vertexShaderState() const noexcept
	-> std::shared_ptr<GpuShaderState>
{
	return mVertexShaderState;
}

auto CodeRed::PipelineInfo::depthStencilState() const noexcept
	-> std::shared_ptr<GpuDepthStencilState>
{
	return mDepthStencilState;
}

auto CodeRed::PipelineInfo::pixelShaderState() const noexcept
	-> std::shared_ptr<GpuShaderState>
{
	return mPixelShaderState;
}

auto CodeRed::PipelineInfo::resourceLayout() const noexcept
	-> std::shared_ptr<GpuResourceLayout>
{
	return mResourceLayout;
}

auto CodeRed::PipelineInfo::blendState() const noexcept
	-> std::shared_ptr<GpuBlendState>
{
	return mBlendState;
}

auto CodeRed::PipelineInfo::renderPass() const noexcept
	-> std::shared_ptr<GpuRenderPass>
{
	return mRenderPass;
}
