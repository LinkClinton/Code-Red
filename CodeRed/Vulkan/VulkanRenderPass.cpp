#include "VulkanLogicalDevice.hpp"
#include "VulkanRenderPass.hpp"

#ifdef __ENABLE__VULKAN__

using namespace CodeRed::Vulkan;

CodeRed::VulkanRenderPass::VulkanRenderPass(
	const std::shared_ptr<GpuLogicalDevice>& device,
	const std::vector<Attachment>& colors, 
	const std::optional<Attachment>& depth) :
	GpuRenderPass(device, colors, depth)
{
	size_t attachmentCount = 0;

	attachmentCount = attachmentCount + mColorAttachments.size();
	attachmentCount = attachmentCount + (mDepthAttachment.has_value() ? 1 : 0);
	
	std::vector<vk::AttachmentDescription> attachments(attachmentCount);
	std::vector<vk::AttachmentReference> colorsReference(mColorAttachments.size());
	vk::AttachmentReference depthReference;
	
	attachmentCount = 0;

	for (size_t index = 0; index < mColorAttachments.size(); index++) {
		const auto& colorAttachment = mColorAttachments[index];

		colorsReference[index].attachment = static_cast<uint32_t>(attachmentCount);
		colorsReference[index].layout = vk::ImageLayout::eColorAttachmentOptimal;
		
		attachments[attachmentCount++]
			.setFlags(vk::AttachmentDescriptionFlags(0))
			.setFormat(enumConvert(colorAttachment.Format))
			.setSamples(enumConvert(colorAttachment.Sample))
			.setLoadOp(enumConvert(colorAttachment.Load))
			.setStoreOp(enumConvert(colorAttachment.Store))
			.setStencilLoadOp(enumConvert(colorAttachment.StencilLoad))
			.setStencilStoreOp(enumConvert(colorAttachment.StencilStore))
			.setInitialLayout(enumConvert(colorAttachment.InitialLayout))
			.setFinalLayout(enumConvert(colorAttachment.FinalLayout));
	}

	CODE_RED_TRY_EXECUTE(
		mDepthAttachment.has_value(),
		attachments[attachmentCount++]
		.setFlags(vk::AttachmentDescriptionFlags(0))
		.setFormat(enumConvert(mDepthAttachment->Format))
		.setSamples(enumConvert(mDepthAttachment->Sample))
		.setLoadOp(enumConvert(mDepthAttachment->Load))
		.setStoreOp(enumConvert(mDepthAttachment->Store))
		.setStencilLoadOp(enumConvert(mDepthAttachment->StencilLoad))
		.setStencilStoreOp(enumConvert(mDepthAttachment->StencilStore))
		.setInitialLayout(enumConvert(mDepthAttachment->InitialLayout))
		.setFinalLayout(enumConvert(mDepthAttachment->FinalLayout))
	);

	CODE_RED_TRY_EXECUTE(
		mDepthAttachment.has_value(),
		depthReference
		.setAttachment(static_cast<uint32_t>(colorsReference.size()))
		.setLayout(vk::ImageLayout::eDepthStencilAttachmentOptimal)
	);
	
	vk::SubpassDescription subPassInfo = {};

	subPassInfo
		.setFlags(vk::SubpassDescriptionFlags(0))
		.setPipelineBindPoint(vk::PipelineBindPoint::eGraphics)
		.setInputAttachmentCount(0)
		.setColorAttachmentCount(static_cast<uint32_t>(colorsReference.size()))
		.setPreserveAttachmentCount(0)
		.setPInputAttachments(nullptr)
		.setPColorAttachments(colorsReference.data())
		.setPPreserveAttachments(nullptr)
		.setPResolveAttachments(nullptr)
		.setPDepthStencilAttachment(mDepthAttachment.has_value() ? &depthReference : nullptr);

	vk::RenderPassCreateInfo info = {};

	info
		.setPNext(nullptr)
		.setFlags(vk::RenderPassCreateFlags(0))
		.setAttachmentCount(static_cast<uint32_t>(attachments.size()))
		.setPAttachments(attachments.data())
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
