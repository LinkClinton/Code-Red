#pragma once

#include "../../Interface/GpuPipelineState/GpuDepthStencilState.hpp"
#include "../VulkanUtility.hpp"

#ifdef __ENABLE__VULKAN__

namespace CodeRed {

	class VulkanDepthStencilState final : public GpuDepthStencilState {
	public:
		explicit VulkanDepthStencilState(
			const std::shared_ptr<GpuLogicalDevice>& device,
			const PixelFormat format,
			const bool depth_enable = true,
			const bool depth_write_enable = true,
			const bool stencil_enable = false,
			const CompareOperator depth_operator = CompareOperator::LessEqual,
			const StencilOperatorInfo& front = StencilOperatorInfo(),
			const StencilOperatorInfo& back = StencilOperatorInfo());

		~VulkanDepthStencilState() = default;

		auto state() const noexcept -> vk::PipelineDepthStencilStateCreateInfo { return mState; }
	private:
		vk::PipelineDepthStencilStateCreateInfo mState = {};
	};
	
}

#endif