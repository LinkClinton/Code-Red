#pragma once

#include "../Interface/GpuResource/GpuTexture.hpp"
#include "../Shared/Information/TextureRefInfo.hpp"
#include "../Shared/Noncopyable.hpp"

#include <memory>

namespace CodeRed {

	class GpuTexture;
	
	class GpuTextureRef : public Noncopyable {
	protected:
		explicit GpuTextureRef(
			const std::shared_ptr<GpuTexture>& texture,
			const TextureRefInfo& info);

		~GpuTextureRef() = default;
	public:
		auto width() const noexcept -> size_t { return mTexture->width(mInfo.MipLevel.Start); }

		auto height() const noexcept -> size_t { return mTexture->height(mInfo.MipLevel.Start); }

		auto depth() const noexcept -> size_t { return mTexture->depth(mInfo.MipLevel.Start); }
		
		auto info() const noexcept -> TextureRefInfo { return mInfo; }

		auto format() const noexcept -> PixelFormat { return mTexture->format(); }

		auto mipLevel() const noexcept -> ValueRange<size_t> { return mInfo.MipLevel; }

		auto array() const noexcept -> ValueRange<size_t> { return mInfo.Array; }

		auto usage() const noexcept -> TextureRefUsage { return mInfo.Usage; }

		auto source() const noexcept -> std::shared_ptr<GpuTexture> { return mTexture; }
	protected:
		std::shared_ptr<GpuTexture> mTexture;
		
		TextureRefInfo mInfo;
	};
	
}