#include "VulkanDepthStencilState.hpp"

#ifdef __ENABLE__VULKAN__

CodeRed::VulkanDepthStencilState::VulkanDepthStencilState(
	const std::shared_ptr<GpuLogicalDevice>& device,
	const PixelFormat format,
	const bool depth_enable,
	const bool depth_write_enable,
	const bool stencil_enable,
	const CompareOperator depth_operator,
	const StencilOperatorInfo& front,
	const StencilOperatorInfo& back) :
	GpuDepthStencilState(
		device,
		format,
		depth_enable,
		depth_write_enable,
		stencil_enable,
		depth_operator,
		front,
		back
	)
{
	mState
		.setPNext(nullptr)
		.setFlags(vk::PipelineDepthStencilStateCreateFlags(0))
		.setDepthTestEnable(mDepthEnable)
		.setDepthWriteEnable(mDepthWriteEnable)
		.setDepthCompareOp(enumConvert(mDepthOperator))
		.setDepthBoundsTestEnable(false)
		.setMinDepthBounds(0)
		.setMaxDepthBounds(0)
		.setStencilTestEnable(mStencilEnable)
		.setFront(vk::StencilOpState(
			enumConvert(mFrontFace.FailOperator),
			enumConvert(mFrontFace.PassOperator),
			enumConvert(mFrontFace.DepthFailOperator),
			enumConvert(mFrontFace.CompareOperator)))
		.setBack(vk::StencilOpState(
			enumConvert(mBackFace.FailOperator),
			enumConvert(mBackFace.PassOperator),
			enumConvert(mBackFace.DepthFailOperator),
			enumConvert(mBackFace.CompareOperator)));
}

#endif