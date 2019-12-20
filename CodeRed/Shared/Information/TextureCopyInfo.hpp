#pragma once

#include <memory>

namespace CodeRed {

	class GpuTexture;
	
	struct TextureCopyInfo {
		std::shared_ptr<GpuTexture> Texture = nullptr;
		size_t ResourceIndex = 0;
		size_t LocationX = 0;
		size_t LocationY = 0;
		size_t LocationZ = 0;

		TextureCopyInfo() = default;
		
		TextureCopyInfo(
			const std::shared_ptr<GpuTexture>& texture,
			const size_t locationX = 0,
			const size_t locationY = 0,
			const size_t locationZ = 0) :
			TextureCopyInfo(texture, 0, locationX, locationY, locationZ) {}

		TextureCopyInfo(
			const std::shared_ptr<GpuTexture>& texture,
			const size_t resourceIndex,
			const size_t locationX,
			const size_t locationY,
			const size_t locationZ) :
			Texture(texture), ResourceIndex(resourceIndex),
			LocationX(locationX), LocationY(locationY), LocationZ(locationZ) {}
	};
	
}