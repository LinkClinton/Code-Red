#include "VulkanRasterizationState.hpp"

#ifdef __ENABLE__VULKAN__

using namespace CodeRed::Vulkan;

CodeRed::VulkanRasterizationState::VulkanRasterizationState(
	const std::shared_ptr<GpuLogicalDevice>& device,
	const FrontFace front_face,
	const CullMode cull_mode, 
	const FillMode fill_mode, 
	const bool depth_clamp) :
	GpuRasterizationState(
		device,
		front_face,
		cull_mode,
		fill_mode,
		depth_clamp
	)
{
	mState
		.setPNext(nullptr)
		.setFlags(vk::PipelineRasterizationStateCreateFlags(0))
		.setPolygonMode(enumConvert(mFillMode))
		.setCullMode(enumConvert(mCullMode))
		.setFrontFace(enumConvert(mFrontFace))
		.setDepthClampEnable(mDepthClamp)
		.setRasterizerDiscardEnable(false)
		.setDepthBiasEnable(false)
		.setDepthBiasConstantFactor(0)
		.setDepthBiasClamp(0)
		.setDepthBiasSlopeFactor(0)
		.setLineWidth(1.0f);
}

#endif