#pragma once

#include "../Shared/Noncopyable.hpp"

#include <memory>

namespace CodeRed {

	enum class PixelFormat;
	
	class GpuLogicalDevice;

	struct WindowInfo {
		size_t width;
		size_t height;

		void* handle;
	};
	
	class GpuSwapChain : public Noncopyable {
	public:
		GpuSwapChain(
			const std::shared_ptr<GpuLogicalDevice>& device,
			const WindowInfo& info,
			const PixelFormat& format) :
			mDevice(device),
			mWindowInfo(info),
			mPixelFormat(format) {}
		
		~GpuSwapChain() = default;
	protected:
		std::shared_ptr<GpuLogicalDevice> mDevice;

		WindowInfo mWindowInfo;
		PixelFormat mPixelFormat;
	};
	
}