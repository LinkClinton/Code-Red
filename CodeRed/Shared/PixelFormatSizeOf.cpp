#include "PixelFormatSizeOf.hpp"

auto CodeRed::PixelFormatSizeOf::get(const PixelFormat pixel_format) -> size_t {
	switch (pixel_format) {
	case PixelFormat::Unknown:
		return 0;
	case PixelFormat::RedGreenBlueAlpha8BitUnknown:
		return 4;
	default:
		return 0;
	}

	return 0;
}
