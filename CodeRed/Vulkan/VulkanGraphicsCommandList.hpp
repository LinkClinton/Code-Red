#pragma once

#include "../Interface/GpuGraphicsCommandList.hpp"
#include "VulkanUtility.hpp"

#ifdef __ENABLE__VULKAN__

namespace CodeRed {

	class VulkanGraphicsCommandList final : public GpuGraphicsCommandList {
	public:
		explicit VulkanGraphicsCommandList(
			const std::shared_ptr<GpuLogicalDevice>& device,
			const std::shared_ptr<GpuCommandAllocator>& allocator);

		~VulkanGraphicsCommandList();

		auto commandList() const noexcept -> vk::CommandBuffer { return mCommandBuffer; }
	private:
		vk::CommandBuffer mCommandBuffer;
	};
	
}

#endif