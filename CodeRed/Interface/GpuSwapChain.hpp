#pragma once

#include "../Shared/Information/WindowInfo.hpp"
#include "../Shared/Enum/PixelFormat.hpp"
#include "../Shared/Noncopyable.hpp"

#include <memory>
#include <vector>

namespace CodeRed {

	class GpuLogicalDevice;
	class GpuTexture;
	
	class GpuSwapChain : public Noncopyable {
	protected:
		explicit GpuSwapChain(
			const std::shared_ptr<GpuLogicalDevice>& device,
			const WindowInfo& info,
			const PixelFormat& format,
			const size_t buffer_count = 2) :
			mDevice(device),
			mBuffers(buffer_count),
			mWindowInfo(info),
			mPixelFormat(format) {}
		
		~GpuSwapChain() = default;
	public:
		auto buffer(const size_t index) const -> std::shared_ptr<GpuTexture> { return mBuffers[index]; }

		auto bufferCount() const noexcept -> size_t { return mBuffers.size(); }

		auto windowInfo() const noexcept -> WindowInfo { return mWindowInfo; }

		auto width() const noexcept -> UInt32 { return mWindowInfo.width; }

		auto height() const noexcept -> UInt32 { return mWindowInfo.height; }

		auto format() const noexcept -> PixelFormat { return mPixelFormat; }
		
		virtual void present(bool sync = true) = 0;

		virtual auto currentBufferIndex() const -> size_t = 0;
	protected:
		std::shared_ptr<GpuLogicalDevice> mDevice;

		std::vector<std::shared_ptr<GpuTexture>> mBuffers;
		
		WindowInfo mWindowInfo;
		PixelFormat mPixelFormat;
	};
	
}
