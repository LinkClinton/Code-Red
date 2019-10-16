#include "VulkanResource/VulkanBuffer.hpp"

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

	//clear the cache resources that used for copy memory to buffer or texture
	//because the command list only record the commands, we need to keep the resource
	//until the queue finishs the copy commands
	//(we need keep the resource even the command list was destroyed)
	mCopyCacheResources.clear();
}

auto CodeRed::VulkanCommandAllocator::allocateCopyCacheBuffer(const size_t size)
	-> std::shared_ptr<GpuBuffer>
{
	const auto buffer = mDevice->createBuffer(
		ResourceInfo::UploadBuffer(size, 1)
	);

	mCopyCacheResources.push_back(buffer);

	return buffer;
}

#endif
