#include "VulkanCommandQueue.hpp"
#include "VulkanLogicalDevice.hpp"

#ifdef __ENABLE__VULKAN__

CodeRed::VulkanCommandQueue::VulkanCommandQueue(
	std::shared_ptr<GpuLogicalDevice>& device) :
	GpuCommandQueue(device)
{
	const auto vkDevice = std::static_pointer_cast<VulkanLogicalDevice>(mDevice);

	mQueueIndex = vkDevice->allocateQueue();

	mQueue = vkDevice->device().getQueue(
		static_cast<uint32_t>(vkDevice->queueFamilyIndex()),
		static_cast<uint32_t>(mQueueIndex));
}

CodeRed::VulkanCommandQueue::~VulkanCommandQueue()
{
	std::static_pointer_cast<VulkanLogicalDevice>(mDevice)->freeQueue(mQueueIndex);
}

#endif
