#include "Exception/NotSupportException.hpp"
#include "PixelFormatSizeOf.hpp"

auto CodeRed::PixelFormatSizeOf::get(const PixelFormat pixel_format) -> size_t {
	switch (pixel_format) {
	case PixelFormat::RedGreenBlueAlpha8BitUnknown:
		return 4;
	case PixelFormat::RedGreenBlueAlpha32BitFloat:
		return 16;
	case PixelFormat::RedGreenBlue32BitFloat:
		return 12;
	case PixelFormat::RedGreen32BitFloat:
		return 8;
	case PixelFormat::Unknown:
		throw NotSupportException(NotSupportException::Type::Enum);
	default:
		throw NotSupportException(NotSupportException::Type::Enum);
	}
}
