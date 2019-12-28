#include "VulkanTextureBuffer.hpp"

#include "../VulkanLogicalDevice.hpp"
#include "VulkanTexture.hpp"

#ifdef __ENABLE__VULKAN__

using namespace CodeRed::Vulkan;

CodeRed::VulkanTextureBuffer::VulkanTextureBuffer(
	const std::shared_ptr<GpuLogicalDevice>& device,
	const TextureBufferInfo& info) : GpuTextureBuffer(device, info)
{
	vk::MemoryAllocateInfo memoryInfo = {};
	vk::BufferCreateInfo bufferInfo = {};

	bufferInfo
		.setPNext(nullptr)
		.setFlags(vk::BufferCreateFlags(0))
		.setUsage(vk::BufferUsageFlagBits::eTransferDst | vk::BufferUsageFlagBits::eTransferSrc)
		.setSize(mInfo.Size)
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
				enumConvert(MemoryHeap::Upload)));

	mMemory = vkDevice->device().allocateMemory(memoryInfo);

	vkDevice->device().bindBufferMemory(mBuffer, mMemory, 0);
}

CodeRed::VulkanTextureBuffer::VulkanTextureBuffer(
	const std::shared_ptr<GpuLogicalDevice>& device,
	const std::shared_ptr<GpuTexture>& texture, 
	const size_t mipSlice) : VulkanTextureBuffer(device, 
		TextureBufferInfo(
			texture->width(mipSlice),
			texture->height(mipSlice),
			texture->depth(mipSlice),
			texture->format(),
			texture->dimension()))
{
}

CodeRed::VulkanTextureBuffer::~VulkanTextureBuffer()
{
	const auto vkDevice = std::static_pointer_cast<VulkanLogicalDevice>(mDevice)->device();

	vkDevice.freeMemory(mMemory);
	vkDevice.destroyBuffer(mBuffer);
}

auto CodeRed::VulkanTextureBuffer::read(const Extent3D<size_t>& extent) const -> std::vector<Byte>
{
	const auto rowPitch = mInfo.Width * PixelFormatSizeOf::get(mInfo.Format);
	const auto depthPitch = rowPitch * mInfo.Height;
	const auto widthOffset = extent.Left * PixelFormatSizeOf::get(mInfo.Format);
	
	std::vector<Byte> data(extent.width() * extent.height() * extent.depth() * PixelFormatSizeOf::get(mInfo.Format));

	const auto vkDevice = std::static_pointer_cast<VulkanLogicalDevice>(mDevice)->device();

	const auto buffer = vkDevice.mapMemory(mMemory, 0, VK_WHOLE_SIZE);

	const auto dataLength = extent.width() * PixelFormatSizeOf::get(mInfo.Format);
	
	size_t dstOffset = 0;

	for (auto z = extent.Front; z < extent.Back; z++) {
		for (auto y = extent.Top; y < extent.Bottom; y++) {
			const auto srcOffset = z * depthPitch + y * rowPitch + widthOffset;

			std::memcpy(data.data() + dstOffset, static_cast<unsigned char*>(buffer) + srcOffset, dataLength);

			dstOffset = dstOffset + dataLength;
		}
	}

	vkDevice.unmapMemory(mMemory);
	
	return data;
}

auto CodeRed::VulkanTextureBuffer::read() const -> std::vector<Byte>
{
	std::vector<Byte> data(mInfo.Size);
	
	const auto vkDevice = std::static_pointer_cast<VulkanLogicalDevice>(mDevice)->device();
	const auto buffer = vkDevice.mapMemory(mMemory, 0, VK_WHOLE_SIZE);

	std::memcpy(data.data(), buffer, mInfo.Size);

	vkDevice.unmapMemory(mMemory);

	return data;
}

void CodeRed::VulkanTextureBuffer::write(const Extent3D<size_t>& extent, const void* data)
{
	const auto rowPitch = mInfo.Width * PixelFormatSizeOf::get(mInfo.Format);
	const auto depthPitch = rowPitch * mInfo.Height;
	const auto widthOffset = extent.Left * PixelFormatSizeOf::get(mInfo.Format);

	const auto vkDevice = std::static_pointer_cast<VulkanLogicalDevice>(mDevice)->device();

	const auto buffer = vkDevice.mapMemory(mMemory, 0, VK_WHOLE_SIZE);

	const auto dataLength = extent.width() * PixelFormatSizeOf::get(mInfo.Format);

	size_t srcOffset = 0;

	for (auto z = extent.Front; z < extent.Back; z++) {
		for (auto y = extent.Top; y < extent.Bottom; y++) {
			const auto dstOffset = z * depthPitch + y * rowPitch + widthOffset;

			std::memcpy(
				static_cast<unsigned char*>(buffer) + dstOffset,
				static_cast<const unsigned char*>(data) + srcOffset, dataLength);

			srcOffset = srcOffset + dataLength;
		}
	}

	vkDevice.unmapMemory(mMemory);
}

void CodeRed::VulkanTextureBuffer::write(const void* data)
{
	const auto vkDevice = std::static_pointer_cast<VulkanLogicalDevice>(mDevice)->device();
	const auto buffer = vkDevice.mapMemory(mMemory, 0, VK_WHOLE_SIZE);

	std::memcpy(buffer, data, mInfo.Size);
	
	vkDevice.unmapMemory(mMemory);
}

#endif
