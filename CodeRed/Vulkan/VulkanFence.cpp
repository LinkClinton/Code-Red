#include "VulkanLogicalDevice.hpp"
#include "VulkanFence.hpp"

#ifdef __ENABLE__VULKAN__

using namespace CodeRed::Vulkan;

CodeRed::VulkanFence::VulkanFence(const std::shared_ptr<GpuLogicalDevice>& device) :
	GpuFence(device)
{
	const auto vkDevice = std::static_pointer_cast<VulkanLogicalDevice>(mDevice)->device();
	
	vk::FenceCreateInfo info = {};

	info
		.setPNext(nullptr)
		.setFlags(vk::FenceCreateFlags(0));

	mFence = vkDevice.createFence(info);
}

CodeRed::VulkanFence::~VulkanFence()
{
	std::static_pointer_cast<VulkanLogicalDevice>(mDevice)->device()
		.destroyFence(mFence);
}

#endif
