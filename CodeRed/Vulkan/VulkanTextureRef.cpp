#include "VulkanTextureRef.hpp"

#include "VulkanResource/VulkanTexture.hpp"

#ifdef __ENABLE__VULKAN__

using namespace CodeRed::Vulkan;

CodeRed::VulkanTextureRef::VulkanTextureRef(
	const std::shared_ptr<GpuTexture>& texture, 
	const TextureRefInfo& info) :
	GpuTextureRef(texture, info)
{
	const auto property = std::get<TextureProperty>(texture->info().Property);
	
	mViewInfo
		.setPNext(nullptr)
		.setFlags(vk::ImageViewCreateFlags(0))
		.setImage(std::static_pointer_cast<VulkanTexture>(texture)->image())
		.setFormat(enumConvert(mInfo.Format == PixelFormat::Unknown ? property.Format : mInfo.Format))
		.setComponents(vk::ComponentMapping(
			vk::ComponentSwizzle::eR,
			vk::ComponentSwizzle::eG,
			vk::ComponentSwizzle::eB,
			vk::ComponentSwizzle::eA))
		.setSubresourceRange(vk::ImageSubresourceRange(
			enumConvert(property.Format, texture->info().Usage),
			static_cast<uint32_t>(mInfo.MipLevel.Start),
			static_cast<uint32_t>(mInfo.MipLevel.size()),
			static_cast<uint32_t>(mInfo.Array.Start),
			static_cast<uint32_t>(mInfo.Array.size())))
		.setViewType(enumConvert(property.Dimension, mInfo.Usage, mInfo.Array.size()));
}

#endif