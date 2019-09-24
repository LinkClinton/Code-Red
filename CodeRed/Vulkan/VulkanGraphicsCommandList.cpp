#include "VulkanGraphicsCommandList.hpp"
#include "VulkanCommandAllocator.hpp"
#include "VulkanLogicalDevice.hpp"

#ifdef __ENABLE__VULKAN__

CodeRed::VulkanGraphicsCommandList::VulkanGraphicsCommandList(
	const std::shared_ptr<GpuLogicalDevice>& device,
	const std::shared_ptr<GpuCommandAllocator>& allocator) :
	GpuGraphicsCommandList(device, allocator)
{
	const auto vkDevice = std::static_pointer_cast<VulkanLogicalDevice>(mDevice)->device();
	const auto vkAllocator = std::static_pointer_cast<VulkanCommandAllocator>(mAllocator)->allocator();
	
	vk::CommandBufferAllocateInfo info = {};

	info
		.setPNext(nullptr)
		.setCommandPool(vkAllocator)
		.setLevel(vk::CommandBufferLevel::ePrimary)
		.setCommandBufferCount(1);

	mCommandBuffer = vkDevice.allocateCommandBuffers(info)[0];
}

CodeRed::VulkanGraphicsCommandList::~VulkanGraphicsCommandList()
{
	const auto vkDevice = std::static_pointer_cast<VulkanLogicalDevice>(mDevice)->device();
	const auto vkAllocator = std::static_pointer_cast<VulkanCommandAllocator>(mAllocator)->allocator();

	vkDevice.freeCommandBuffers(vkAllocator, mCommandBuffer);
}

#endif
