#pragma once

#ifdef __CODE__RED__GLOBAL__INCLUDE__
#include <Shared/Information/WindowInfo.hpp>
#include <Shared/Enum/PixelFormat.hpp>
#include <Shared/Noncopyable.hpp>
#else
#include "../Shared/Information/WindowInfo.hpp"
#include "../Shared/Enum/PixelFormat.hpp"
#include "../Shared/Noncopyable.hpp"
#endif

#include <memory>
#include <vector>

namespace CodeRed {

	class GpuLogicalDevice;
	class GpuCommandQueue;
	class GpuTexture;
	
	class GpuSwapChain : public Noncopyable {
	protected:
		explicit GpuSwapChain(
			const std::shared_ptr<GpuLogicalDevice>& device,
			const std::shared_ptr<GpuCommandQueue>& queue,
			const WindowInfo& info,
			const PixelFormat& format,
			const size_t buffer_count = 2);
		
		~GpuSwapChain() = default;
	public:
		auto buffer(const size_t index) const -> std::shared_ptr<GpuTexture> { return mBuffers[index]; }

		auto bufferCount() const noexcept -> size_t { return mBuffers.size(); }

		auto info() const noexcept -> WindowInfo { return mInfo; }

		auto width() const noexcept -> size_t { return mInfo.width; }

		auto height() const noexcept -> size_t { return mInfo.height; }

		auto format() const noexcept -> PixelFormat { return mPixelFormat; }

		virtual void resize(const size_t width, const size_t height) = 0;
		
		virtual void present(bool sync = false) = 0;

		virtual auto currentBufferIndex() const -> size_t = 0;
	protected:
		std::shared_ptr<GpuLogicalDevice> mDevice;
		std::shared_ptr<GpuCommandQueue> mQueue;

		std::vector<std::shared_ptr<GpuTexture>> mBuffers;
		
		WindowInfo mInfo;
		PixelFormat mPixelFormat;
	};
	
}
