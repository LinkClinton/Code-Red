#include "VulkanBlendState.hpp"

#ifdef __ENABLE__VULKAN__

using namespace CodeRed::Vulkan;

CodeRed::VulkanBlendState::VulkanBlendState(
	const std::shared_ptr<GpuLogicalDevice>& device,
	const BlendProperty& blend_property) :
	GpuBlendState(device, blend_property),
	mAttachments(mBlendProperties.size())
{	
	const auto& property = mBlendProperties[0];
	
	mAttachments[0]
		.setColorWriteMask(enumConvert(property.ColorMask))
		.setBlendEnable(property.Enable)
		.setAlphaBlendOp(enumConvert(property.AlphaOperator))
		.setColorBlendOp(enumConvert(property.ColorOperator))
		.setSrcColorBlendFactor(enumConvert(property.Source))
		.setDstColorBlendFactor(enumConvert(property.Destination))
		.setSrcAlphaBlendFactor(enumConvert(property.SourceAlpha))
		.setDstAlphaBlendFactor(enumConvert(property.DestinationAlpha));

	mState
		.setPNext(nullptr)
		.setFlags(vk::PipelineColorBlendStateCreateFlags(0))
		.setAttachmentCount(static_cast<uint32_t>(mBlendProperties.size()))
		.setPAttachments(mAttachments.data())
		.setLogicOpEnable(false)
		.setLogicOp(vk::LogicOp::eNoOp)
		.setBlendConstants({ 1.0f,1.0f,1.0f,1.0f });
}


#endif
