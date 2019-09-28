#pragma once

#include "../Utility.hpp"

namespace CodeRed {

	enum class PixelFormat : UInt32
	{
		Unknown,
		RedGreenBlueAlpha8BitUnknown,
		RedGreenBlueAlpha32BitFloat,
		RedGreenBlue32BitFloat,
		RedGreen32BitFloat
	};
	
}