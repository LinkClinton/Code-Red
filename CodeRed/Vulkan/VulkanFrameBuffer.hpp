#pragma once

#include "../Interface/GpuFrameBuffer.hpp"
#include "VulkanUtility.hpp"

#ifdef __ENABLE__VULKAN__

namespace CodeRed {

	class VulkanFrameBuffer final : public GpuFrameBuffer {
	public:
		explicit VulkanFrameBuffer(
			const std::shared_ptr<GpuLogicalDevice>& device,
			const std::vector<std::shared_ptr<GpuTextureRef>>& render_targets,
			const std::shared_ptr<GpuTextureRef>& depth_stencil = nullptr);
		
		~VulkanFrameBuffer();

		auto frameBuffer() const noexcept -> vk::Framebuffer { return mFrameBuffer; }

		auto width() const noexcept -> size_t { return mWidth; }

		auto height() const noexcept -> size_t { return mHeight; }
	private:
		vk::Framebuffer mFrameBuffer;
		vk::ImageView mDepthStencilView;
		
		std::vector<vk::ImageView> mRenderTargetView;
		
		std::shared_ptr<GpuRenderPass> mRenderPass;

		size_t mWidth = 0;
		size_t mHeight = 0;
	};
	
}

#endif