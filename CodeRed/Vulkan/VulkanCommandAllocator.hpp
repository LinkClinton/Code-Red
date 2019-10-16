#pragma once

#include "../Interface/GpuCommandAllocator.hpp"
#include "VulkanUtility.hpp"

#ifdef __ENABLE__VULKAN__

namespace CodeRed {

	class GpuBuffer;
	
	class VulkanCommandAllocator final : public GpuCommandAllocator {
	public:
		explicit VulkanCommandAllocator(
			const std::shared_ptr<GpuLogicalDevice>& device);

		~VulkanCommandAllocator();

		void reset() override;
		
		auto allocator() const noexcept -> vk::CommandPool {return mCommandPool; }
	private:
		friend class VulkanGraphicsCommandList;

		auto allocateCopyCacheBuffer(const size_t size) -> std::shared_ptr<GpuBuffer>;
	private:
		vk::CommandPool mCommandPool;
	};
	
}

#endif