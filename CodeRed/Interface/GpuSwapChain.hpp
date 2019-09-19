#pragma once

#include <Shared/Information/WindowInfo.hpp>
#include <Shared/Enum/PixelFormat.hpp>
#include <Shared/Noncopyable.hpp>

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
			const size_t buffer_count = 2);
		
		~GpuSwapChain() = default;
	public:
		auto buffer(const size_t index) const -> std::shared_ptr<GpuTexture> { return mBuffers[index]; }

		auto bufferCount() const noexcept -> size_t { return mBuffers.size(); }

		auto info() const noexcept -> WindowInfo { return mInfo; }

		auto width() const noexcept -> UInt32 { return mInfo.width; }

		auto height() const noexcept -> UInt32 { return mInfo.height; }

		auto format() const noexcept -> PixelFormat { return mPixelFormat; }

		virtual void resize(const UInt32 width, const UInt32 height) = 0;
		
		virtual void present(bool sync = true) = 0;

		virtual auto currentBufferIndex() const -> size_t = 0;
	protected:
		std::shared_ptr<GpuLogicalDevice> mDevice;

		std::vector<std::shared_ptr<GpuTexture>> mBuffers;
		
		WindowInfo mInfo;
		PixelFormat mPixelFormat;
	};
	
}
