#include "../Shared/Exception/ZeroException.hpp"

#include "VulkanResource/VulkanTexture.hpp"

#include "VulkanLogicalDevice.hpp"
#include "VulkanFrameBuffer.hpp"
#include "VulkanRenderPass.hpp"
#include "VulkanTextureRef.hpp"

#undef max
#undef min

#ifdef __ENABLE__VULKAN__

using namespace CodeRed::Vulkan;

CodeRed::VulkanFrameBuffer::VulkanFrameBuffer(
	const std::shared_ptr<GpuLogicalDevice>& device,
	const std::shared_ptr<GpuTexture>& render_target, 
	const std::shared_ptr<GpuTexture>& depth_stencil) :
	VulkanFrameBuffer(device, 
		render_target->reference(), 
		depth_stencil->reference())
{
}

CodeRed::VulkanFrameBuffer::VulkanFrameBuffer(
	const std::shared_ptr<GpuLogicalDevice>& device,
	const std::shared_ptr<GpuTextureRef>& render_target, 
	const std::shared_ptr<GpuTextureRef>& depth_stencil) :
	GpuFrameBuffer(device, render_target, depth_stencil), mRenderTargetView(1)
{
	VulkanFrameBuffer::reset(render_target, depth_stencil);
}

CodeRed::VulkanFrameBuffer::~VulkanFrameBuffer()
{
	const auto vkDevice = std::static_pointer_cast<VulkanLogicalDevice>(mDevice)->device();

	for (auto& renderTargetView : mRenderTargetView)
		if (renderTargetView) vkDevice.destroyImageView(renderTargetView);

	if (mDepthStencilView) vkDevice.destroyImageView(mDepthStencilView);
	
	vkDevice.destroyFramebuffer(mFrameBuffer);
}

void CodeRed::VulkanFrameBuffer::reset(
	const std::shared_ptr<GpuTextureRef>& render_target,
	const std::shared_ptr<GpuTextureRef>& depth_stencil)
{
	const auto vkDevice = std::static_pointer_cast<VulkanLogicalDevice>(mDevice)->device();

	if (mFrameBuffer) vkDevice.destroyFramebuffer(mFrameBuffer);

	for (auto& rtv : mRenderTargets) rtv.reset();

	mDepthStencil.reset();
	mRenderPass.reset();

	auto& renderTarget = mRenderTargets[0] = render_target;
	auto& depthStencil = mDepthStencil = depth_stencil;

	//warning, when we create a frame buffer without rtv and dsv
	//only output when we enable __ENABLE__CODE__RED__DEBUG__
	CODE_RED_DEBUG_TRY_EXECUTE(
		renderTarget == nullptr &&
		depthStencil == nullptr,
		DebugReport::warning(DebugType::Create, { "FrameBuffer", "there are no rtv and dsv" })
	);
	
	using OptAttachment = std::optional<Attachment>;

	mRenderPass = std::make_shared<VulkanRenderPass>(
		mDevice,
		renderTarget != nullptr ? static_cast<OptAttachment>(
			Attachment::RenderTarget(renderTarget->source()->format())) : std::nullopt,
		depthStencil != nullptr ? static_cast<OptAttachment>(
			Attachment::DepthStencil(depthStencil->source()->format())) : std::nullopt);

	//create the frame buffer
	vk::FramebufferCreateInfo info = {};
	vk::ImageView views[2] = { nullptr, nullptr };

	uint32_t attachmentCount = 0;

	if (renderTarget != nullptr) {
		mRenderTargetView[0] = vkDevice.createImageView(std::static_pointer_cast<VulkanTextureRef>(renderTarget)->viewInfo());

		views[attachmentCount++] = mRenderTargetView[0];
	}

	if (depthStencil != nullptr) {
		mDepthStencilView = vkDevice.createImageView(std::static_pointer_cast<VulkanTextureRef>(depthStencil)->viewInfo());

		views[attachmentCount++] = mDepthStencilView;
	}

	auto& width = mFrameBufferWidth = 1;
	auto& height = mFrameBufferHeight = 1;

	//get the width and height of frame buffer
	//if we have render target, the width and height is render target
	//else if we have depth stencil, the width and height is depth stencil
	//else width and height is 1
	CODE_RED_TRY_EXECUTE(renderTarget != nullptr, width = std::max(width, renderTarget->source()->width(renderTarget->mipLevel().Start)));
	CODE_RED_TRY_EXECUTE(renderTarget != nullptr, height = std::max(height, renderTarget->source()->height(renderTarget->mipLevel().Start)));

	CODE_RED_TRY_EXECUTE(
		renderTarget == nullptr &&
		depthStencil != nullptr,
		width = std::max(width, depthStencil->source()->width(depthStencil->mipLevel().Start)));


	CODE_RED_TRY_EXECUTE(
		renderTarget == nullptr &&
		depthStencil != nullptr,
		height = std::max(width, depthStencil->source()->height(depthStencil->mipLevel().Start)));

	
	info
		.setPNext(nullptr)
		.setFlags(vk::FramebufferCreateFlags(0))
		.setRenderPass(std::static_pointer_cast<VulkanRenderPass>(mRenderPass)->renderPass())
		.setAttachmentCount(attachmentCount)
		.setPAttachments(views)
		.setWidth(static_cast<uint32_t>(width))
		.setHeight(static_cast<uint32_t>(height))
		.setLayers(1);

	mFrameBuffer = vkDevice.createFramebuffer(info);
}

#endif
