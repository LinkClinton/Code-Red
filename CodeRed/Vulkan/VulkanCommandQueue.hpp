#pragma once

#include "../Interface/GpuCommandQueue.hpp"
#include "VulkanUtility.hpp"

#ifdef __ENABLE__VULKAN__

namespace CodeRed {

	class VulkanCommandQueue final : public GpuCommandQueue {
	public:
		explicit VulkanCommandQueue(
			const std::shared_ptr<GpuLogicalDevice>& device);

		~VulkanCommandQueue();

		void execute(const std::vector<std::shared_ptr<GpuGraphicsCommandList>>& lists) override;

		void waitIdle() override;
		
		auto queue() const noexcept -> vk::Queue { return mQueue; }
	private:
		vk::Queue mQueue;

		size_t mQueueIndex = SIZE_MAX;
	};
	
}

#endif