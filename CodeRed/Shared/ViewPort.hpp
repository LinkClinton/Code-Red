#pragma once

#ifdef __CODE__RED__GLOBAL__INCLUDE__
#include <Shared/Utility.hpp>
#else
#include "Utility.hpp"
#endif

namespace CodeRed {

	struct ViewPort {
		Real X = 0;
		Real Y = 0;

		Real Width = 0;
		Real Height = 0;

		Real MinDepth = 0;
		Real MaxDepth = 1;
	};
	
}