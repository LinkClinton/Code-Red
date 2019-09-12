#include "PixelFormatSizeOf.hpp"
#include "Exception/NotSupportException.hpp"

auto CodeRed::PixelFormatSizeOf::get(const PixelFormat pixel_format) -> size_t {
	switch (pixel_format) {
	case PixelFormat::RedGreenBlueAlpha8BitUnknown:
		return 4;
	case PixelFormat::Unknown:
		throw NotSupportException(NotSupportException::Type::Enum);
	default:
		throw NotSupportException(NotSupportException::Type::Enum);
	}
}
