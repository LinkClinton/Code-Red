#pragma once

#include "../Interface/GpuCommandAllocator.hpp"
#include "VulkanUtility.hpp"

#ifdef __ENABLE__VULKAN__

namespace CodeRed {

	class VulkanCommandAllocator final : public GpuCommandAllocator {
	public:
		explicit VulkanCommandAllocator(
			const std::shared_ptr<GpuLogicalDevice>& device);

		~VulkanCommandAllocator();

		auto allocator() const noexcept -> vk::CommandPool {return mCommandPool; }
	private:
		vk::CommandPool mCommandPool;
	};
	
}

#endif