#pragma once

#include "../../Interface/GpuResource/GpuSampler.hpp"
#include "../VulkanUtility.hpp"

#ifdef __ENABLE__VULKAN__

namespace CodeRed {

	class VulkanSampler final : public GpuSampler {
	public:
		explicit VulkanSampler(
			const std::shared_ptr<GpuLogicalDevice>& device,
			const SamplerInfo& info);

		~VulkanSampler();

		auto sampler() const noexcept -> vk::Sampler { return mSampler; }
	private:
		vk::Sampler mSampler;
	};
	
}

#endif