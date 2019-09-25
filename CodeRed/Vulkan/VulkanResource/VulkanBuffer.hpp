#pragma once

#include "../../Interface/GpuResource/GpuBuffer.hpp"
#include "../VulkanUtility.hpp"

#ifdef __ENABLE__VULKAN__

namespace CodeRed {

	class VulkanBuffer final : public GpuBuffer {
	public:
		explicit VulkanBuffer(
			const std::shared_ptr<GpuLogicalDevice>& device,
			const ResourceInfo& info);

		~VulkanBuffer();

		auto buffer() const noexcept -> vk::Buffer { return mBuffer; }
	private:
		vk::DeviceMemory mMemory;
		vk::Buffer mBuffer;
	};
	
}

#endif