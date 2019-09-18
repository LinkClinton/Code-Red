#pragma once

#include <Shared/Utility.hpp>

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