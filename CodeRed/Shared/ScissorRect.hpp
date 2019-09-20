#pragma once

#ifdef __CODE__RED__GLOBAL__INCLUDE__
#include <Shared/Utility.hpp>
#else
#include "Utility.hpp"
#endif

namespace CodeRed {

	struct ScissorRect {
		size_t Left = 0;
		size_t Top = 0;
		size_t Right = 0;
		size_t Bottom = 0;
	};
	
}