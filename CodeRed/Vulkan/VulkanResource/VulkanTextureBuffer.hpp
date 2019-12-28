#pragma once

#include "../../Interface/GpuResource/GpuTextureBuffer.hpp"
#include "../VulkanUtility.hpp"

#ifdef __ENABLE__VULKAN__

namespace CodeRed {

	class VulkanTextureBuffer final : public GpuTextureBuffer {
	public:
		explicit VulkanTextureBuffer(
			const std::shared_ptr<GpuLogicalDevice>& device,
			const TextureBufferInfo& info);

		explicit VulkanTextureBuffer(
			const std::shared_ptr<GpuLogicalDevice>& device,
			const std::shared_ptr<GpuTexture>& texture,
			const size_t mipSlice = 0);

		~VulkanTextureBuffer();

		auto read(const Extent3D<size_t>& extent) const -> std::vector<Byte> override;

		auto read() const -> std::vector<Byte> override;
		
		void write(const Extent3D<size_t>& extent, const void* data) override;

		void write(const void* data) override;
		
		auto buffer() const noexcept -> vk::Buffer { return mBuffer; }
	private:
		vk::DeviceMemory mMemory;
		vk::Buffer mBuffer;
	};
	
}

#endif