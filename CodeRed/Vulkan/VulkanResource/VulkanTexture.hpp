#pragma once

#include "../../Interface/GpuResource/GpuTexture.hpp"
#include "../VulkanUtility.hpp"

#ifdef __ENABLE__VULKAN__

namespace CodeRed {

	class VulkanTexture final : public GpuTexture {
	public:
		explicit VulkanTexture(
			const std::shared_ptr<GpuLogicalDevice>& device,
			const ResourceInfo& info);

		explicit VulkanTexture(
			const std::shared_ptr<GpuLogicalDevice>& device,
			const ResourceInfo& info,
			const vk::Image image);
		
		~VulkanTexture();

		auto image() const noexcept -> vk::Image { return mImage; }

		auto view() const noexcept -> vk::ImageView { return mImageView; }
	private:
		vk::DeviceMemory mMemory;
		vk::ImageView mImageView;
		vk::Image mImage;
	};
	
}

#endif