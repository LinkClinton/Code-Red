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

#endif
