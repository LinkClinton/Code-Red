#pragma once

#include <memory>

namespace CodeRed {

	class GpuTextureBuffer;
	
	struct TextureBufferCopyInfo {
		std::shared_ptr<GpuTextureBuffer> Buffer = nullptr;

		TextureBufferCopyInfo() = default;

		TextureBufferCopyInfo(const std::shared_ptr<GpuTextureBuffer>& buffer) :
			Buffer(buffer) {}
	};
	
}