#include "Exception/NotSupportException.hpp"
#include "PixelFormatSizeOf.hpp"

auto CodeRed::PixelFormatSizeOf::get(const PixelFormat pixel_format) -> size_t {
	switch (pixel_format) {
	case PixelFormat::RedGreenBlueAlpha8BitUnknown:
		return 4;
	case PixelFormat::BlueGreenRedAlpha8BitUnknown:
		return 4;
	case PixelFormat::RedGreenBlueAlpha32BitFloat:
		return 16;
	case PixelFormat::RedGreenBlue32BitFloat:
		return 12;
	case PixelFormat::RedGreen32BitFloat:
		return 8;
	case PixelFormat::Depth32BitFloat:
		return 4;
	case PixelFormat::Unknown:
		throw NotSupportException(NotSupportType::Enum);
	default:
		throw NotSupportException(NotSupportType::Enum);
	}
}

bool CodeRed::PixelFormatSizeOf::isDepthOnly(const PixelFormat pixelFormat)
{
	switch (pixelFormat) {
	case PixelFormat::Depth32BitFloat: return true;
	default: return false;
	}
}
