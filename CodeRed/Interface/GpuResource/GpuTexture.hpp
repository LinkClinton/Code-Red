#pragma once

#include "../../Shared/Exception/NotSupportException.hpp"
#include "../../Shared/ValueRange.hpp"

#include "GpuResource.hpp"

namespace CodeRed {
	
	class GpuTexture : public GpuResource {
	protected:
		explicit GpuTexture(
			const std::shared_ptr<GpuLogicalDevice>& device,
			const ResourceInfo& info);

		explicit GpuTexture(
			const std::shared_ptr<GpuLogicalDevice>& device,
			const ValueRange<size_t>& arrayRange,
			const ValueRange<size_t>& mipRange,
			const ResourceInfo& info);
		
		~GpuTexture() = default;
	public:
		virtual auto reference(
			const ValueRange<size_t> arrayRange,
			const ValueRange<size_t> mipRange) const -> std::shared_ptr<GpuTexture> = 0;

		auto reference(const ValueRange<size_t>& arrayRange) const -> std::shared_ptr<GpuTexture> {
			return reference(arrayRange, { 0, mipLevels() });
		}
		
		auto width(const size_t mipSlice) const noexcept -> size_t;

		auto height(const size_t mipSlice) const noexcept -> size_t;

		auto depth(const size_t mipSlice) const noexcept -> size_t;
		
		auto size(const size_t mipSlice) const noexcept -> size_t;
		
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

		/*
		 * these functions are related to the range of mip level and array
		 */
		
		auto width() const noexcept -> size_t { return width(mMipRange.Start); }

		auto height() const noexcept -> size_t { return height(mMipRange.Start); }

		auto depth() const noexcept -> size_t { return depth(mMipRange.Start); }

		auto size() const noexcept -> size_t { return size(mMipRange.Start); }

		auto mipRange() const noexcept -> ValueRange<size_t> { return mMipRange; }

		auto arrayRange() const noexcept -> ValueRange<size_t> { return mArrayRange; }
	protected:
		size_t mPhysicalSize = 0;
		size_t mAlignment = 0;

		ValueRange<size_t> mArrayRange;
		ValueRange<size_t> mMipRange;
	};

}