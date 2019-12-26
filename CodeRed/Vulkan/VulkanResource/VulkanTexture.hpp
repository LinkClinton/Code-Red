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

		auto reference(const TextureRefInfo& info) -> std::shared_ptr<GpuTextureRef> override;
		
		auto image() const noexcept -> vk::Image { return mImage; }
	private:
		vk::DeviceMemory mMemory;
		vk::Image mImage;
	};
	
}

#endif