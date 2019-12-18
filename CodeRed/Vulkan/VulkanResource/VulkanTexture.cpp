#include "../../Shared/PixelFormatSizeOf.hpp"
#include "../VulkanLogicalDevice.hpp"
#include "VulkanTexture.hpp"


#ifdef __ENABLE__VULKAN__

using namespace CodeRed::Vulkan;

CodeRed::VulkanTexture::VulkanTexture(
	const std::shared_ptr<GpuLogicalDevice>& device, 
	const ResourceInfo& info) :
	GpuTexture(device, info)
{
	vk::ImageViewCreateInfo viewInfo = {};
	vk::MemoryAllocateInfo memoryInfo = {};
	vk::ImageCreateInfo imageInfo = {};

	const auto property = std::get<TextureProperty>(mInfo.Property);
	
	imageInfo
		.setPNext(nullptr)
		.setFlags(vk::ImageCreateFlags(0))
		.setImageType(enumConvert(property.Dimension))
		.setFormat(enumConvert(property.PixelFormat))
		.setExtent(vk::Extent3D(
			static_cast<uint32_t>(property.Width),
			static_cast<uint32_t>(property.Height),
			/*
			 * if we use the texture array, the depth must be 1, but we use depth to store the size of array
			 * so we can not use depth when the texture is not Texture3D
			 */
			static_cast<uint32_t>(property.Dimension == Dimension::Dimension3D ? property.Depth : 1)))
		.setMipLevels(1)
		/*
		 * https://vulkan.lunarg.com/doc/view/latest/windows/apispec.html#VkImageCreateInfo
		 * it limits the size of array layers must be 1 when the texture is Texture3D
		 */
		.setArrayLayers(static_cast<uint32_t>(property.Dimension == Dimension::Dimension3D ? 1 : property.Depth))
		.setSamples(vk::SampleCountFlagBits::e1)
		.setInitialLayout(enumConvert(mInfo.Layout))
		.setUsage(enumConvert(mInfo.Usage).second)
		.setQueueFamilyIndexCount(0)
		.setPQueueFamilyIndices(nullptr)
		.setSharingMode(vk::SharingMode::eExclusive);

	const auto vkDevice = std::static_pointer_cast<VulkanLogicalDevice>(mDevice);

	mImage = vkDevice->device().createImage(imageInfo);

	const auto memoryRequirement = vkDevice->device().getImageMemoryRequirements(mImage);

	//because the size of texture is not always same in different adapters
	//so we need record the real size and the alignment
	mPhysicalSize = memoryRequirement.size;
	mAlignment = memoryRequirement.alignment;
	
	memoryInfo
		.setPNext(nullptr)
		.setAllocationSize(mPhysicalSize)
		.setMemoryTypeIndex(
			vkDevice->getMemoryTypeIndex(memoryRequirement.memoryTypeBits,
				enumConvert(mInfo.Heap)));

	mMemory = vkDevice->device().allocateMemory(memoryInfo);

	vkDevice->device().bindImageMemory(mImage, mMemory, 0);

	viewInfo
		.setPNext(nullptr)
		.setFlags(vk::ImageViewCreateFlags(0))
		.setImage(mImage)
		.setFormat(enumConvert(property.PixelFormat))
		.setComponents(vk::ComponentMapping(
			vk::ComponentSwizzle::eR,
			vk::ComponentSwizzle::eG,
			vk::ComponentSwizzle::eB,
			vk::ComponentSwizzle::eA))
		.setSubresourceRange(vk::ImageSubresourceRange(
			enumConvert(property.PixelFormat, mInfo.Usage),
			0, 1, 0, 
			static_cast<uint32_t>(property.Dimension == Dimension::Dimension3D ? 1 : property.Depth)))
		.setViewType(enumConvert(property.Dimension, mInfo.Type, property.Depth));
	
	mImageView = vkDevice->device().createImageView(viewInfo);
}

CodeRed::VulkanTexture::VulkanTexture(
	const std::shared_ptr<GpuLogicalDevice>& device, 
	const ResourceInfo& info,
	const vk::Image image) :
	GpuTexture(device, info),
	mMemory(nullptr),
	mImage(image)
{
	//this ctor version is used for swapchain
	//we use this ctor to create GpuTexture in swap chain.
	const auto vkDevice = std::static_pointer_cast<VulkanLogicalDevice>(mDevice);
	const auto property = std::get<TextureProperty>(mInfo.Property);

	auto imageAspectFlags = vk::ImageAspectFlags(0);

	if (enumHas(mInfo.Usage, ResourceUsage::DepthStencil))
		imageAspectFlags = vk::ImageAspectFlagBits::eDepth | vk::ImageAspectFlagBits::eStencil;
	else imageAspectFlags = vk::ImageAspectFlagBits::eColor;


	vk::ImageViewCreateInfo viewInfo = {};
	
	viewInfo
		.setPNext(nullptr)
		.setFlags(vk::ImageViewCreateFlags(0))
		.setImage(mImage)
		.setFormat(enumConvert(property.PixelFormat))
		.setComponents(vk::ComponentMapping(
			vk::ComponentSwizzle::eR,
			vk::ComponentSwizzle::eG,
			vk::ComponentSwizzle::eB,
			vk::ComponentSwizzle::eA))
		.setSubresourceRange(vk::ImageSubresourceRange(
			imageAspectFlags,
			0, 1, 0, 1))
		.setViewType(enumConvert1(property.Dimension));

	mImageView = vkDevice->device().createImageView(viewInfo);
}

CodeRed::VulkanTexture::~VulkanTexture()
{
	const auto vkDevice = std::static_pointer_cast<VulkanLogicalDevice>(mDevice)->device();

	vkDevice.destroyImageView(mImageView);

	//vulkan texture for swapchain
	//so we do not need to destroy memory and image
	//we will do this when we destroy the swapchain
	if (mMemory) {
		vkDevice.freeMemory(mMemory);
		vkDevice.destroyImage(mImage);
	}
}

#endif
