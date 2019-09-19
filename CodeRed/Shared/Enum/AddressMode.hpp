#pragma once

#ifdef __CODE__RED__GLOBAL__INCLUDE__
#include <Shared/Utility.hpp>
#else
#include "../Utility.hpp"
#endif

namespace CodeRed {

	enum class AddressMode : UInt32
	{
		Repeat,
		Mirror,
		Clamp,
		Border
	};
	
}