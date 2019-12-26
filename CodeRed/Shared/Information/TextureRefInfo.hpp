#pragma once

#include "../Enum/TextureRefUsage.hpp"

#include "../ValueRange.hpp"

namespace CodeRed {

	struct TextureRefInfo {
		ValueRange<size_t> MipLevel = ValueRange<size_t>(0, 0);
		ValueRange<size_t> Array = ValueRange<size_t>(0, 0);

		TextureRefUsage Usage = TextureRefUsage::Common;

		TextureRefInfo() = default;

		TextureRefInfo(
			const ValueRange<size_t>& mipLevel,
			const ValueRange<size_t>& array,
			const TextureRefUsage& usage = TextureRefUsage::Common) :
			MipLevel(mipLevel), Array(array), Usage(usage) {}
	};
	
}