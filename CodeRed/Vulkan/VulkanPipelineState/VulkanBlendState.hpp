#pragma once

#include "../../Interface/GpuPipelineState/GpuBlendState.hpp"
#include "../VulkanUtility.hpp"

#ifdef __ENABLE__VULKAN__

namespace CodeRed {

	class VulkanBlendState final : public GpuBlendState {
	public:
		explicit VulkanBlendState(
			const std::shared_ptr<GpuLogicalDevice>& device,
			const std::vector<BlendProperty>& blend_properties);

		~VulkanBlendState() = default;

		auto state() const noexcept -> vk::PipelineColorBlendStateCreateInfo { return mState; }
	private:
		std::vector<vk::PipelineColorBlendAttachmentState> mAttachments = {};
		vk::PipelineColorBlendStateCreateInfo mState = {};
	};
	
}

#endif