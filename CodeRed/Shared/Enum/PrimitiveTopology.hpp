#pragma once

#ifdef __CODE__RED__GLOBAL__INCLUDE__
#include <Shared/Utility.hpp>
#else
#include "../Utility.hpp"
#endif

namespace CodeRed {

	enum class PrimitiveTopology : UInt32
	{
		Undefined,
		TriangleList
	};
	
}