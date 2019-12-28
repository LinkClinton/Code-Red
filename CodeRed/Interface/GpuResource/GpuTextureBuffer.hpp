#pragma once

#include "../../Shared/Information/TextureBufferInfo.hpp"
#include "../../Shared/Noncopyable.hpp"
#include "../../Shared/Extent.hpp"

#include <vector>
#include <memory>

namespace CodeRed {

	class GpuLogicalDevice;
	class GpuTexture;
	
	class GpuTextureBuffer : public Noncopyable {
	protected:
		explicit GpuTextureBuffer(
			const std::shared_ptr<GpuLogicalDevice>& device,
			const TextureBufferInfo& info);

		~GpuTextureBuffer() = default;
	public:
		auto info() const noexcept -> TextureBufferInfo { return mInfo; }

		auto size() const noexcept -> size_t { return mInfo.Size; }
		
		auto width() const noexcept -> size_t { return mInfo.Width; }

		auto height() const noexcept -> size_t { return mInfo.Height; }

		auto depth() const noexcept -> size_t { return mInfo.Depth; }

		auto format() const noexcept -> PixelFormat { return mInfo.Format; }

		auto layout() const noexcept -> ResourceLayout { return mInfo.Layout; }
		
		auto dimension() const noexcept -> Dimension { return mInfo.Dimension; }

		auto physicalSize() const -> size_t { return mPhysicalSize; }

		auto alignment() const -> size_t { return mAlignment; }

		void write(const std::vector<Byte>& data) { write(data.data()); }

		void write(const Extent3D<size_t>& extent, const std::vector<Byte>& data) { write(extent, data.data()); }

		virtual auto read(const Extent3D<size_t>& extent) const -> std::vector<Byte> = 0;
		
		virtual auto read() const -> std::vector<Byte> = 0;

		virtual void write(const Extent3D<size_t>& extent, const void* data) = 0;
		
		virtual void write(const void* data) = 0;
	protected:
		friend class DirectX12GraphicsCommandList;
		friend class VulkanGraphicsCommandList;

		void setLayout(const ResourceLayout layout) { mInfo.Layout = layout; }
	protected:
		std::shared_ptr<GpuLogicalDevice> mDevice;

		TextureBufferInfo mInfo;

		size_t mPhysicalSize = 0;
		size_t mAlignment = 0;
	};
	
}