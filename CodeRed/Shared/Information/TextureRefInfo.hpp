#pragma once

#include "../Enum/TextureRefUsage.hpp"
#include "../Enum/PixelFormat.hpp"

#include "../ValueRange.hpp"

namespace CodeRed {

	struct TextureRefInfo {
		ValueRange<size_t> MipLevel = ValueRange<size_t>(0, 0);
		ValueRange<size_t> Array = ValueRange<size_t>(0, 0);

		TextureRefUsage Usage = TextureRefUsage::Common;
		PixelFormat Format = PixelFormat::Unknown;

		TextureRefInfo() = default;

		TextureRefInfo(const TextureRefUsage& usage, const PixelFormat& format = PixelFormat::Unknown) :
			Usage(usage), Format(format) {}
		
		TextureRefInfo(
			const ValueRange<size_t>& mipLevel,
			const ValueRange<size_t>& array,
			const TextureRefUsage& usage = TextureRefUsage::Common,
			const PixelFormat& format = PixelFormat::Unknown) :
			MipLevel(mipLevel), Array(array), Usage(usage), Format(format) {}
	};
	
}