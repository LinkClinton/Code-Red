#pragma once

#ifdef __CODE__RED__GLOBAL__INCLUDE__
#include <Shared/Utility.hpp>
#else
#include "Utility.hpp"
#endif

namespace CodeRed {

	struct ScissorRect {
		UInt32 Left = 0;
		UInt32 Top = 0;
		UInt32 Right = 0;
		UInt32 Bottom = 0;
	};
	
}