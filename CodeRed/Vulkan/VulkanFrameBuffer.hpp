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

		~VulkanFrameBuffer();

		void reset(
			const std::shared_ptr<GpuTexture>& render_target, 
			const std::shared_ptr<GpuTexture>& depth_stencil) override;
		
		auto frameBuffer() const noexcept -> vk::Framebuffer { return mFrameBuffer; }
	private:
		vk::Framebuffer mFrameBuffer;
		vk::RenderPass mRenderPass;
	};
	
}

#endif