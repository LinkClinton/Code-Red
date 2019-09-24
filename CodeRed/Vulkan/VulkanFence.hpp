#pragma once

#include "../Interface/GpuFence.hpp"
#include "VulkanUtility.hpp"

#ifdef __ENABLE__VULKAN__

namespace CodeRed {

	class VulkanFence final : public GpuFence {
	public:
		explicit VulkanFence(
			const std::shared_ptr<GpuLogicalDevice>& device);

		~VulkanFence();

		auto fence() const noexcept -> vk::Fence { return mFence; }
	private:
		vk::Fence mFence;
	};
	
}

#endif