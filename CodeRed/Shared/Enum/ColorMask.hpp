#pragma once

#include <Shared/Utility.hpp>

namespace CodeRed {

	enum class ColorMask : UInt32
	{
		None = 0,
		Red = 1 << 0,
		Green = 1 << 1,
		Blue = 1 << 2,
		Alpha = 1 << 3,
		All = Red | Green | Blue | Alpha
	};

	inline ColorMask operator | (const ColorMask &left, const ColorMask& right) {
		return static_cast<ColorMask>(
			static_cast<UInt32>(left) |
			static_cast<UInt32>(right));
	}
	
}