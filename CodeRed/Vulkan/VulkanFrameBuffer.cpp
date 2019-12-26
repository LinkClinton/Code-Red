#include "../Shared/Exception/ZeroException.hpp"

#include "VulkanResource/VulkanTexture.hpp"

#include "VulkanLogicalDevice.hpp"
#include "VulkanFrameBuffer.hpp"
#include "VulkanRenderPass.hpp"

#undef max
#undef min

#ifdef __ENABLE__VULKAN__

using namespace CodeRed::Vulkan;

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

	vkDevice.destroyFramebuffer(mFrameBuffer);
}

void CodeRed::VulkanFrameBuffer::reset(
	const std::shared_ptr<GpuTexture>& render_target,
	const std::shared_ptr<GpuTexture>& depth_stencil)
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

	CODE_RED_DEBUG_THROW_IF(
		mRenderTargets[0]->arrayRange().size() != 1 || mRenderTargets[0]->mipRange().size() != 1 ||
		mDepthStencil->arrayRange().size() != 1 || mDepthStencil->mipRange().size() != 1,
		"The range of array and mip level is not one, we will use the first texture as frame buffer."
	);
	
	using OptAttachment = std::optional<Attachment>;

	mRenderPass = std::make_shared<VulkanRenderPass>(
		mDevice,
		renderTarget != nullptr ? static_cast<OptAttachment>(Attachment::RenderTarget(renderTarget->format())) : std::nullopt,
		depthStencil != nullptr ? static_cast<OptAttachment>(Attachment::DepthStencil(depthStencil->format())) : std::nullopt);

	//create the frame buffer
	vk::FramebufferCreateInfo info = {};
	vk::ImageView views[2] = { nullptr, nullptr };

	uint32_t attachmentCount = 0;

	CODE_RED_TRY_EXECUTE(
		renderTarget != nullptr,
		views[attachmentCount++] = std::static_pointer_cast<VulkanTexture>(renderTarget)->view();
	);

	CODE_RED_TRY_EXECUTE(
		depthStencil != nullptr,
		views[attachmentCount++] = std::static_pointer_cast<VulkanTexture>(depthStencil)->view();
	);

	auto& width = mFrameBufferWidth = 1;
	auto& height = mFrameBufferHeight = 1;

	//get the width and height of frame buffer
	//if we have render target, the width and height is render target
	//else if we have depth stencil, the width and height is depth stencil
	//else width and height is 1
	CODE_RED_TRY_EXECUTE(renderTarget != nullptr, width = std::max(width, renderTarget->width()));
	CODE_RED_TRY_EXECUTE(renderTarget != nullptr, height = std::max(height, renderTarget->height()));

	CODE_RED_TRY_EXECUTE(
		renderTarget == nullptr &&
		depthStencil != nullptr,
		width = std::max(width, depthStencil->width()));


	CODE_RED_TRY_EXECUTE(
		renderTarget == nullptr &&
		depthStencil != nullptr,
		height = std::max(width, depthStencil->height()));

	
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
