#pragma once

#include "../../Shared/Exception/NotSupportException.hpp"
#include "GpuResource.hpp"

namespace CodeRed {
	
	class GpuTexture : public GpuResource {
	protected:
		explicit GpuTexture(
			const std::shared_ptr<GpuLogicalDevice>& device,
			const ResourceInfo& resource_info) :
			GpuResource(device, resource_info) {}

		~GpuTexture() = default;
	public:
		auto size() const noexcept -> size_t override { return std::get<TextureProperty>(mResourceInfo.Property).Size; }

		auto format() const noexcept -> PixelFormat { return std::get<TextureProperty>(mResourceInfo.Property).PixelFormat; }

		auto dimension() const noexcept -> Dimension { return std::get<TextureProperty>(mResourceInfo.Property).Dimension; }

		auto width() const noexcept -> size_t { return std::get<TextureProperty>(mResourceInfo.Property).Width; }

		auto height() const -> size_t {
			//only for texture2D or texture3D
			throwIf(dimension() != Dimension::Dimension1D, NotSupportException(NotSupportType::Method));

			return std::get<TextureProperty>(mResourceInfo.Property).Height;
		}

		auto depth() const -> size_t {
			//only for texture3D
			throwIf(dimension() == Dimension::Dimension3D, NotSupportException(NotSupportType::Method));

			return std::get<TextureProperty>(mResourceInfo.Property).Depth;
		}
	};

}