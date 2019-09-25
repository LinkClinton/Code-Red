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
	private:
		void initializeSwapChain();
	private:
		vk::SwapchainKHR mSwapChain;
		vk::SurfaceKHR mSurface;
	};
	
}

#endif