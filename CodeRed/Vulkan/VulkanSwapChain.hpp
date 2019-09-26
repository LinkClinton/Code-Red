#pragma once

#include "../Interface/GpuSwapChain.hpp"
#include "VulkanUtility.hpp"

#ifdef __ENABLE__VULKAN__

namespace CodeRed {

	class VulkanSwapChain final : public GpuSwapChain {
	public:
		explicit VulkanSwapChain(
			const std::shared_ptr<GpuLogicalDevice>& device,
			const std::shared_ptr<GpuCommandQueue>& queue,
			const WindowInfo& info,
			const PixelFormat& format,
			const size_t buffer_count = 2);

		~VulkanSwapChain();

		void resize(const size_t width, const size_t height) override;

		void present() override;

		auto currentBufferIndex() const -> size_t override;

		auto swapChain() const noexcept -> vk::SwapchainKHR { return mSwapChain; }

		auto surface() const noexcept -> vk::SurfaceKHR { return mSurface; }
	private:
		void initializeSwapChain();

		void updateCurrentFrameIndex();
	private:
		vk::SwapchainKHR mSwapChain;
		vk::SurfaceKHR mSurface;
		vk::Semaphore mSemaphore;

		uint32_t mCurrentBufferIndex = 0;
	};
	
}

#endif