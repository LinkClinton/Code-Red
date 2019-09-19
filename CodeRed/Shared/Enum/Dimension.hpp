#pragma once

#ifdef __CODE__RED__GLOBAL__INCLUDE__
#include <Shared/Utility.hpp>
#else
#include "../Utility.hpp"
#endif

namespace CodeRed {

	enum class Dimension : UInt32
	{
		Dimension1D,
		Dimension2D,
		Dimension3D,
	};
	
}