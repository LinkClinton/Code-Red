#pragma once

#include <memory>

namespace CodeRed {

	class GpuTexture;
	
	struct TextureResolveInfo {
		std::shared_ptr<GpuTexture> Texture;
		size_t ArraySlice = 0;
		size_t MipSlice = 0;

		TextureResolveInfo() = default;

		TextureResolveInfo(
			const std::shared_ptr<GpuTexture>& texture,
			const size_t arraySlice) :
			TextureResolveInfo(texture, arraySlice, 0) {}

		TextureResolveInfo(
			const std::shared_ptr<GpuTexture>& texture,
			const size_t arraySlice,
			const size_t mipSlice) :
			Texture(texture), ArraySlice(arraySlice), MipSlice(mipSlice) {}
	};
	
}
