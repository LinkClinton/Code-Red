#include "../VulkanLogicalDevice.hpp"
#include "VulkanSampler.hpp"

#ifdef __ENABLE__VULKAN__

CodeRed::VulkanSampler::VulkanSampler(
	const std::shared_ptr<GpuLogicalDevice>& device, 
	const SamplerInfo& info) :
	GpuSampler(device, info)
{
	vk::SamplerCreateInfo samplerInfo = {};

	samplerInfo
		.setPNext(nullptr)
		.setFlags(vk::SamplerCreateFlags(0))
		.setMagFilter(enumConvert(mInfo.Filter, 1))
		.setMinFilter(enumConvert(mInfo.Filter, 0))
		.setMipmapMode(enumConvert(mInfo.Filter))
		.setAddressModeU(enumConvert(mInfo.AddressModeU))
		.setAddressModeV(enumConvert(mInfo.AddressModeV))
		.setAddressModeW(enumConvert(mInfo.AddressModeW))
		.setMipLodBias(0)
		.setAnisotropyEnable(mInfo.Filter == FilterOptions::Anisotropy)
		.setMaxAnisotropy(static_cast<float>(mInfo.MaxAnisotropy))
		.setCompareEnable(true)
		.setCompareOp(vk::CompareOp::eLessOrEqual)
		.setMinLod(0.0f)
		.setMaxLod(1.0f)
		.setBorderColor(enumConvert(mInfo.Border))
		.setUnnormalizedCoordinates(false);

	const auto vkDevice = std::static_pointer_cast<VulkanLogicalDevice>(mDevice)->device();

	mSampler = vkDevice.createSampler(samplerInfo);
}

CodeRed::VulkanSampler::~VulkanSampler()
{
	std::static_pointer_cast<VulkanLogicalDevice>(mDevice)->device()
		.destroySampler(mSampler);
}

#endif
