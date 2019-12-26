#pragma once

#include "../Interface/GpuFrameBuffer.hpp"
#include "VulkanUtility.hpp"

#ifdef __ENABLE__VULKAN__

namespace CodeRed {

	class VulkanFrameBuffer final : public GpuFrameBuffer {
	public:
		explicit VulkanFrameBuffer(
			const std::shared_ptr<GpuLogicalDevice>& device,
			const std::shared_ptr<GpuTexture>& render_target,
			const std::shared_ptr<GpuTexture>& depth_stencil = nullptr);

		explicit VulkanFrameBuffer(
			const std::shared_ptr<GpuLogicalDevice>& device,
			const std::shared_ptr<GpuTextureRef>& render_target,
			const std::shared_ptr<GpuTextureRef>& depth_stencil = nullptr);
		
		~VulkanFrameBuffer();

		void reset(
			const std::shared_ptr<GpuTextureRef>& render_target, 
			const std::shared_ptr<GpuTextureRef>& depth_stencil) override;
		
		auto frameBuffer() const noexcept -> vk::Framebuffer { return mFrameBuffer; }

		auto frameBufferWidth() const noexcept -> size_t { return mFrameBufferWidth; }

		auto frameBufferHeight() const noexcept -> size_t { return mFrameBufferHeight; }
	private:
		vk::Framebuffer mFrameBuffer;
		vk::ImageView mDepthStencilView;
		
		std::vector<vk::ImageView> mRenderTargetView;
		
		std::shared_ptr<GpuRenderPass> mRenderPass;

		size_t mFrameBufferWidth;
		size_t mFrameBufferHeight;
	};
	
}

#endif