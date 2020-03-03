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
	const std::vector<std::shared_ptr<GpuTextureRef>>& render_targets,
	const std::shared_ptr<GpuTextureRef>& depth_stencil) :
	GpuFrameBuffer(device, render_targets, depth_stencil)
{
	const auto vkDevice = std::static_pointer_cast<VulkanLogicalDevice>(mDevice)->device();

	//warning, when we create a frame buffer without rtv and dsv
	//only output when we enable __ENABLE__CODE__RED__DEBUG__
	CODE_RED_DEBUG_TRY_EXECUTE(
		mRenderTargets.empty() &&
		mDepthStencil == nullptr,
		DebugReport::warning(DebugType::Create, { "FrameBuffer", "there are no rtv and dsv" })
	);

	std::vector<Attachment> colorAttachments;
	std::optional<Attachment> depthAttachment = 
		mDepthStencil == nullptr ? std::nullopt : std::optional<Attachment>(Attachment::DepthStencil(mDepthStencil->format()));

	for (size_t index = 0; index < mRenderTargets.size(); index++) 
		colorAttachments.push_back(Attachment::RenderTarget(mRenderTargets[index]->format()));

	mRenderPass = std::make_shared<VulkanRenderPass>(mDevice, colorAttachments, depthAttachment);

	std::vector<vk::ImageView> views = {};
	vk::FramebufferCreateInfo info = {};

	for (size_t index = 0; index < mRenderTargets.size(); index++) {
		mRenderTargetView.push_back(vkDevice.createImageView(
			std::static_pointer_cast<VulkanTextureRef>(mRenderTargets[index])->viewInfo()));

		views.push_back(mRenderTargetView[index]);

		mWidth = std::max(mWidth, mRenderTargets[index]->width());
		mHeight = std::max(mHeight, mRenderTargets[index]->height());
	}

	if (mDepthStencil != nullptr) {
		mDepthStencilView = vkDevice.createImageView(
			std::static_pointer_cast<VulkanTextureRef>(mDepthStencil)->viewInfo());

		views.push_back(mDepthStencilView);

		mWidth = std::max(mWidth, mDepthStencil->width());
		mHeight = std::max(mHeight, mDepthStencil->height());
	}

	mWidth = std::max(mWidth, 1LLU);
	mHeight = std::max(mHeight, 1LLU);

	info
		.setPNext(nullptr)
		.setFlags(vk::FramebufferCreateFlags(0))
		.setRenderPass(std::static_pointer_cast<VulkanRenderPass>(mRenderPass)->renderPass())
		.setAttachmentCount(static_cast<uint32_t>(views.size()))
		.setPAttachments(views.data())
		.setWidth(static_cast<uint32_t>(mWidth))
		.setHeight(static_cast<uint32_t>(mHeight))
		.setLayers(1);

	mFrameBuffer = vkDevice.createFramebuffer(info);
}

CodeRed::VulkanFrameBuffer::~VulkanFrameBuffer()
{
	const auto vkDevice = std::static_pointer_cast<VulkanLogicalDevice>(mDevice)->device();

	for (auto& renderTargetView : mRenderTargetView)
		if (renderTargetView) vkDevice.destroyImageView(renderTargetView);

	if (mDepthStencilView) vkDevice.destroyImageView(mDepthStencilView);
	
	vkDevice.destroyFramebuffer(mFrameBuffer);
}

#endif
