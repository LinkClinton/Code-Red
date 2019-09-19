#pragma once

#ifdef __CODE__RED__GLOBAL__INCLUDE__
#include <Shared/Enum/PixelFormat.hpp>
#else
#include "Enum/PixelFormat.hpp"
#endif

namespace CodeRed {

	class PixelFormatSizeOf {
	public:
		PixelFormatSizeOf() = delete;
		
		static auto get(const PixelFormat pixel_format) -> size_t;
	};
	
}