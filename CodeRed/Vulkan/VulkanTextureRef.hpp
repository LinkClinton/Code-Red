#pragma once

#include "../Interface/GpuTextureRef.hpp"
#include "VulkanUtility.hpp"

#ifdef __ENABLE__VULKAN__

namespace CodeRed {

	class VulkanTextureRef final : public GpuTextureRef {
	public:
		explicit VulkanTextureRef(
			const std::shared_ptr<GpuTexture>& texture,
			const TextureRefInfo& info);

		~VulkanTextureRef() = default;

		auto viewInfo() const noexcept -> vk::ImageViewCreateInfo { return mViewInfo; }
	private:
		vk::ImageViewCreateInfo mViewInfo;
	};
	
}

#endif