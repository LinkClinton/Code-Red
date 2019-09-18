#pragma once

#include <Shared/Enum/PixelFormat.hpp>

namespace CodeRed {

	class PixelFormatSizeOf {
	public:
		PixelFormatSizeOf() = delete;
		
		static auto get(const PixelFormat pixel_format) -> size_t;
	};
	
}