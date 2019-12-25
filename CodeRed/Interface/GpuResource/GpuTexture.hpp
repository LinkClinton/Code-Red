#pragma once

#include "../../Shared/Exception/NotSupportException.hpp"

#include "GpuResource.hpp"

namespace CodeRed {
	
	class GpuTexture : public GpuResource {
	protected:
		explicit GpuTexture(
			const std::shared_ptr<GpuLogicalDevice>& device,
			const ResourceInfo& info);

		~GpuTexture() = default;
	public:
		auto width(const size_t mipSlice = 0) const noexcept -> size_t;

		auto height(const size_t mipSlice = 0) const noexcept -> size_t;

		auto depth(const size_t mipSlice = 0) const noexcept -> size_t;
		
		auto size(const size_t mipSlice = 0) const noexcept -> size_t;

		auto format() const noexcept -> PixelFormat { return std::get<TextureProperty>(mInfo.Property).PixelFormat; }

		auto dimension() const noexcept -> Dimension { return std::get<TextureProperty>(mInfo.Property).Dimension; }

		auto clearValue() const noexcept -> ClearValue { return std::get<TextureProperty>(mInfo.Property).ClearValue; }

		auto mipLevels() const noexcept -> size_t { return std::get<TextureProperty>(mInfo.Property).MipLevels; }
		
		auto isArray() const noexcept
			-> bool { return dimension() != Dimension::Dimension3D && std::get<TextureProperty>(mInfo.Property).Depth != 1; }

		auto index(const size_t mipSlice = 0, const size_t arraySlice = 0) const noexcept
			-> size_t { return arraySlice * mipLevels() + mipSlice; }
		
		auto physicalSize() const -> size_t { return mPhysicalSize; }

		auto alignment() const -> size_t { return mAlignment; }
	protected:
		size_t mPhysicalSize = 0;
		size_t mAlignment = 0;
	};

}