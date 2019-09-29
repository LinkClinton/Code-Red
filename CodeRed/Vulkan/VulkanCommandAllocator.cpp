#include "VulkanCommandAllocator.hpp"
#include "VulkanLogicalDevice.hpp"

#ifdef __ENABLE__VULKAN__

using namespace CodeRed::Vulkan;

CodeRed::VulkanCommandAllocator::VulkanCommandAllocator(
	const std::shared_ptr<GpuLogicalDevice>& device)
	: GpuCommandAllocator(device)
{
	const auto vkDevice = std::static_pointer_cast<VulkanLogicalDevice>(mDevice);

	vk::CommandPoolCreateInfo info = {};
	
	info
		.setPNext(nullptr)
		.setFlags(vk::CommandPoolCreateFlagBits::eResetCommandBuffer)
		.setQueueFamilyIndex(static_cast<uint32_t>(vkDevice->queueFamilyIndex()));

	mCommandPool = vkDevice->device().createCommandPool(info);
}

CodeRed::VulkanCommandAllocator::~VulkanCommandAllocator()
{
	std::static_pointer_cast<VulkanLogicalDevice>(mDevice)->device()
		.destroyCommandPool(mCommandPool);

}

void CodeRed::VulkanCommandAllocator::reset()
{
	std::static_pointer_cast<VulkanLogicalDevice>(mDevice)->device()
		.resetCommandPool(mCommandPool, vk::CommandPoolResetFlagBits::eReleaseResources);
}

#endif
