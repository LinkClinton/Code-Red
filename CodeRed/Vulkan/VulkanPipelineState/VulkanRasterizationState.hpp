#pragma once

#include "../../Interface/GpuPipelineState/GpuRasterizationState.hpp"
#include "../VulkanUtility.hpp"

#ifdef __ENABLE__VULKAN__

namespace CodeRed {

	class VulkanRasterizationState final : public GpuRasterizationState {
	public:
		explicit VulkanRasterizationState(
			const std::shared_ptr<GpuLogicalDevice>& device,
			const FrontFace front_face = FrontFace::CounterClockwise,
			const CullMode cull_mode = CullMode::Back,
			const FillMode fill_mode = FillMode::Solid,
			const bool depth_clamp = true);

		~VulkanRasterizationState() = default;

		auto state() const noexcept -> vk::PipelineRasterizationStateCreateInfo { return mState; }
	private:
		vk::PipelineRasterizationStateCreateInfo mState = {};
	};
	
}

#endif