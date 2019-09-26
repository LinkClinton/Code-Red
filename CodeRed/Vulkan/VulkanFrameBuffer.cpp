#include "../Shared/Exception/ZeroException.hpp"

#include "VulkanResource/VulkanTexture.hpp"

#include "VulkanLogicalDevice.hpp"
#include "VulkanFrameBuffer.hpp"

#ifdef __ENABLE__VULKAN__

CodeRed::VulkanFrameBuffer::VulkanFrameBuffer(
	const std::shared_ptr<GpuLogicalDevice>& device,
	const std::shared_ptr<GpuTexture>& render_target, 
	const std::shared_ptr<GpuTexture>& depth_stencil) :
	GpuFrameBuffer(device, render_target, depth_stencil)
{
	VulkanFrameBuffer::reset(render_target, depth_stencil);
}

CodeRed::VulkanFrameBuffer::~VulkanFrameBuffer()
{
	const auto vkDevice = std::static_pointer_cast<VulkanLogicalDevice>(mDevice)->device();

	vkDevice.destroyRenderPass(mRenderPass);
	vkDevice.destroyFramebuffer(mFrameBuffer);
}

void CodeRed::VulkanFrameBuffer::reset(
	const std::shared_ptr<GpuTexture>& render_target,
	const std::shared_ptr<GpuTexture>& depth_stencil)
{
	CODE_RED_DEBUG_THROW_IF(
		render_target == nullptr,
		ZeroException<GpuTexture>({ "render_target" })
	);

	const auto vkDevice = std::static_pointer_cast<VulkanLogicalDevice>(mDevice)->device();

	//reset the render pass and frame buffer
	if (mFrameBuffer) vkDevice.destroyFramebuffer(mFrameBuffer);
	if (mRenderPass) vkDevice.destroyRenderPass(mRenderPass);
	
	//reset the render target and depth stencil
	for (auto& rtv : mRenderTargets) rtv.reset();

	mDepthStencil.reset();

	mRenderTargets[0] = render_target;

	if (depth_stencil != nullptr) mDepthStencil = depth_stencil;

	//create render pass
	mRenderPass = createRenderPass(vkDevice,
		enumConvert(mRenderTargets[0]->format()),
		mDepthStencil == nullptr ? vk::Format::eUndefined : enumConvert(mDepthStencil->format()));

	//create the frame buffer
	vk::FramebufferCreateInfo info = {};
	vk::ImageView views[2];

	const uint32_t attachmentCount = mDepthStencil == nullptr ? 1 : 2;
	
	views[0] = std::static_pointer_cast<VulkanTexture>(mRenderTargets[0])->view();
	views[1] = mDepthStencil == nullptr ? nullptr : std::static_pointer_cast<VulkanTexture>(mDepthStencil)->view();

	info
		.setPNext(nullptr)
		.setFlags(vk::FramebufferCreateFlags(0))
		.setRenderPass(mRenderPass)
		.setAttachmentCount(attachmentCount)
		.setPAttachments(views)
		.setWidth(static_cast<uint32_t>(mRenderTargets[0]->width()))
		.setHeight(static_cast<uint32_t>(mRenderTargets[0]->height()))
		.setLayers(1);

	mFrameBuffer = vkDevice.createFramebuffer(info);
}

#endif
