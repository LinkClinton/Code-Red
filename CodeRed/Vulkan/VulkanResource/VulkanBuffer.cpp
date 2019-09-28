#include "../VulkanLogicalDevice.hpp"
#include "VulkanBuffer.hpp"

#ifdef __ENABLE__VULKAN__

using namespace CodeRed::Vulkan;

CodeRed::VulkanBuffer::VulkanBuffer(
	const std::shared_ptr<GpuLogicalDevice>& device,
	const ResourceInfo& info) :
	GpuBuffer(device, info)
{
	vk::MemoryAllocateInfo memoryInfo = {};
	vk::BufferCreateInfo bufferInfo = {};

	bufferInfo
		.setPNext(nullptr)
		.setFlags(vk::BufferCreateFlags(0))
		.setUsage(enumConvert(mInfo.Usage).first)
		.setSize(std::get<BufferProperty>(mInfo.Property).Size)
		.setQueueFamilyIndexCount(0)
		.setPQueueFamilyIndices(nullptr)
		.setSharingMode(vk::SharingMode::eExclusive);

	const auto vkDevice = std::static_pointer_cast<VulkanLogicalDevice>(mDevice);

	mBuffer = vkDevice->device().createBuffer(bufferInfo);

	const auto memoryRequirement = vkDevice->device().getBufferMemoryRequirements(mBuffer);

	memoryInfo
		.setPNext(nullptr)
		.setAllocationSize(memoryRequirement.size)
		.setMemoryTypeIndex(
			vkDevice->getMemoryTypeIndex(memoryRequirement.memoryTypeBits,
			enumConvert(mInfo.Heap)));

	mMemory = vkDevice->device().allocateMemory(memoryInfo);

	vkDevice->device().bindBufferMemory(mBuffer, mMemory, 0);
}

CodeRed::VulkanBuffer::~VulkanBuffer()
{
	const auto vkDevice = std::static_pointer_cast<VulkanLogicalDevice>(mDevice)->device();

	vkDevice.freeMemory(mMemory);
	vkDevice.destroyBuffer(mBuffer);
}

auto CodeRed::VulkanBuffer::mapMemory() const -> void* 
{
	const auto vkDevice = std::static_pointer_cast<VulkanLogicalDevice>(mDevice)->device();

	return vkDevice.mapMemory(mMemory, 0, VK_WHOLE_SIZE);
}

void CodeRed::VulkanBuffer::unmapMemory() const
{
	const auto vkDevice = std::static_pointer_cast<VulkanLogicalDevice>(mDevice)->device();

	vkDevice.unmapMemory(mMemory);
}

#endif
