#pragma once

#include "../Utility.hpp"

namespace CodeRed {

	enum class FilterOptions : UInt32
	{
		MinPointMagPointMipPoint = 0,
		MinPointMagPointMipLinear = 1,
		MinPointMagLinearMipPoint = 2,
		MinPointMagLinearMipLinear = 3,
		MinLinearMagPointMipPoint = 4,
		MinLinearMagPointMipLinear = 5,
		MinLinearMagLinearMipPoint = 6,
		MinLinearMagLinearMipLinear = 7,
		Anisotropy = 8
	};
	
}