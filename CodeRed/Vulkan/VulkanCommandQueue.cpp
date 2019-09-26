#include "VulkanGraphicsCommandList.hpp"
#include "VulkanLogicalDevice.hpp"
#include "VulkanCommandQueue.hpp"

#ifdef __ENABLE__VULKAN__

CodeRed::VulkanCommandQueue::VulkanCommandQueue(
	const std::shared_ptr<GpuLogicalDevice>& device) :
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

void CodeRed::VulkanCommandQueue::execute(
	const std::vector<std::shared_ptr<GpuGraphicsCommandList>>& lists)
{
	std::vector<vk::CommandBuffer> vkLists;

	for (auto& list : lists) {
		vkLists.push_back(
			std::static_pointer_cast<VulkanGraphicsCommandList>(list)->commandList()
		);
	}

	vk::SubmitInfo info = {};

	info
		.setPNext(nullptr)
		.setWaitSemaphoreCount(0)
		.setSignalSemaphoreCount(0)
		.setPWaitSemaphores(nullptr)
		.setPSignalSemaphores(nullptr)
		.setPWaitDstStageMask(nullptr)
		.setCommandBufferCount(static_cast<uint32_t>(vkLists.size()))
		.setPCommandBuffers(vkLists.data());

	mQueue.submit(info, nullptr);
}

void CodeRed::VulkanCommandQueue::waitIdle()
{
	mQueue.waitIdle();
}

#endif
