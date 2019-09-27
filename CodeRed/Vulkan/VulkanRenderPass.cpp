#include "VulkanLogicalDevice.hpp"
#include "VulkanRenderPass.hpp"

#ifdef __ENABLE__VULKAN__

CodeRed::VulkanRenderPass::VulkanRenderPass(
	const std::shared_ptr<GpuLogicalDevice>& device,
	const std::optional<Attachment>& color,
	const std::optional<Attachment>& depth) :
	GpuRenderPass(device, color, depth)
{
	const auto& colorAttachment = mColorAttachments[0];
	const auto& depthAttachment = mDepthAttachment;
	
	size_t attachmentCount = 0;
	
	CODE_RED_TRY_EXECUTE(colorAttachment.has_value(), attachmentCount++);
	CODE_RED_TRY_EXECUTE(depthAttachment.has_value(), attachmentCount++);
	
	std::vector<vk::AttachmentDescription> attachments(attachmentCount);

	attachmentCount = 0;
	
	CODE_RED_TRY_EXECUTE(
		colorAttachment.has_value(),
		attachments[attachmentCount++]
		.setFlags(vk::AttachmentDescriptionFlags(0))
		.setFormat(enumConvert(colorAttachment->Format))
		.setSamples(vk::SampleCountFlagBits::e1)
		.setLoadOp(enumConvert(colorAttachment->Load))
		.setStoreOp(enumConvert(colorAttachment->Store))
		.setStencilLoadOp(enumConvert(colorAttachment->StencilLoad))
		.setStencilStoreOp(enumConvert(colorAttachment->StencilStore))
		.setInitialLayout(enumConvert(colorAttachment->InitialLayout))
		.setFinalLayout(enumConvert(colorAttachment->FinalLayout))
	);

	CODE_RED_TRY_EXECUTE(
		depthAttachment.has_value(),
		attachments[attachmentCount++]
		.setFlags(vk::AttachmentDescriptionFlags(0))
		.setFormat(enumConvert(depthAttachment->Format))
		.setSamples(vk::SampleCountFlagBits::e1)
		.setLoadOp(enumConvert(depthAttachment->Load))
		.setStoreOp(enumConvert(depthAttachment->Store))
		.setStencilLoadOp(enumConvert(depthAttachment->StencilLoad))
		.setStencilStoreOp(enumConvert(depthAttachment->StencilStore))
		.setInitialLayout(enumConvert(depthAttachment->InitialLayout))
		.setFinalLayout(enumConvert(depthAttachment->FinalLayout))
	);
	
	vk::AttachmentReference colorReference = {};
	vk::AttachmentReference depthReference = {};

	colorReference.attachment = 0;
	depthReference.layout = vk::ImageLayout::eColorAttachmentOptimal;

	colorReference.attachment = 1;
	depthReference.layout = vk::ImageLayout::eDepthStencilAttachmentOptimal;

	vk::SubpassDescription subPassInfo = {};

	subPassInfo
		.setFlags(vk::SubpassDescriptionFlags(0))
		.setPipelineBindPoint(vk::PipelineBindPoint::eGraphics)
		.setInputAttachmentCount(0)
		.setColorAttachmentCount(colorAttachment.has_value() ? 1 : 0)
		.setPreserveAttachmentCount(0)
		.setPInputAttachments(nullptr)
		.setPColorAttachments(colorAttachment.has_value() ? &colorReference : nullptr)
		.setPPreserveAttachments(nullptr)
		.setPResolveAttachments(nullptr)
		.setPDepthStencilAttachment(depthAttachment.has_value() ? &depthReference : nullptr);

	vk::RenderPassCreateInfo info = {};

	info
		.setPNext(nullptr)
		.setFlags(vk::RenderPassCreateFlags(0))
		.setAttachmentCount(static_cast<uint32_t>(attachments.size()))
		.setPAttachments(attachments.empty() ? nullptr : attachments.data())
		.setSubpassCount(1)
		.setPSubpasses(&subPassInfo)
		.setDependencyCount(0)
		.setPDependencies(nullptr);

	const auto vkDevice = std::static_pointer_cast<VulkanLogicalDevice>(mDevice)->device();
	
	mRenderPass = vkDevice.createRenderPass(info);
}

CodeRed::VulkanRenderPass::~VulkanRenderPass()
{
	const auto vkDevice = std::static_pointer_cast<VulkanLogicalDevice>(mDevice)->device();

	vkDevice.destroyRenderPass(mRenderPass);
}

#endif
