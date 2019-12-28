#pragma once

#include "../../Shared/Exception/NotSupportException.hpp"
#include "../../Shared/Information/TextureRefInfo.hpp"

#include "GpuResource.hpp"

namespace CodeRed {

	class GpuTextureRef;
	
	class GpuTexture : public GpuResource {
	protected:
		explicit GpuTexture(
			const std::shared_ptr<GpuLogicalDevice>& device,
			const ResourceInfo& info);

		~GpuTexture() = default;
	public:
		virtual auto reference(const TextureRefInfo& info) -> std::shared_ptr<GpuTextureRef> = 0;

		auto reference() -> std::shared_ptr<GpuTextureRef> { return reference(TextureRefInfo()); }
		
		auto width(const size_t mipSlice = 0) const noexcept -> size_t;

		auto height(const size_t mipSlice = 0) const noexcept -> size_t;

		auto depth(const size_t mipSlice = 0) const noexcept -> size_t;
		
		auto size(const size_t mipSlice = 0) const noexcept -> size_t;

		auto format() const noexcept -> PixelFormat { return std::get<TextureProperty>(mInfo.Property).Format; }

		auto dimension() const noexcept -> Dimension { return std::get<TextureProperty>(mInfo.Property).Dimension; }

		auto clearValue() const noexcept -> ClearValue { return std::get<TextureProperty>(mInfo.Property).ClearValue; }

		auto mipLevels() const noexcept -> size_t { return std::get<TextureProperty>(mInfo.Property).MipLevels; }

		auto arrays() const noexcept -> size_t { return dimension() == Dimension::Dimension3D ? 1 : depth(); }
		
		auto isArray() const noexcept -> bool { return arrays() != 1; }

		auto index(const size_t mipSlice = 0, const size_t arraySlice = 0) const noexcept
			-> size_t { return arraySlice * mipLevels() + mipSlice; }
		
		auto physicalSize() const -> size_t { return mPhysicalSize; }

		auto alignment() const -> size_t { return mAlignment; }
	protected:
		size_t mPhysicalSize = 0;
		size_t mAlignment = 0;
	};

}