#pragma once

#ifdef __CODE__RED__GLOBAL__INCLUDE__
#include <Shared/Utility.hpp>
#else
#include "../Utility.hpp"
#endif

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