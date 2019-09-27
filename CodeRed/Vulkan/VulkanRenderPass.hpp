#pragma once

#include "../Interface/GpuRenderPass.hpp"
#include "VulkanUtility.hpp"

#ifdef __ENABLE__VULKAN__

namespace CodeRed {

	class VulkanRenderPass final : public GpuRenderPass {
	public:
		explicit VulkanRenderPass(
			const std::shared_ptr<GpuLogicalDevice>& device,
			const std::optional<Attachment>& color,
			const std::optional<Attachment>& depth = std::nullopt);

		~VulkanRenderPass();

		auto renderPass() const noexcept -> vk::RenderPass { return mRenderPass; }
	private:
		vk::RenderPass mRenderPass;
	};
	
}

#endif