#pragma once

#include "../Utility.hpp"

namespace CodeRed {

	enum class FilterOptions : UInt32
	{
		MinPointMagPointMipPoint,
		MinPointMagPointMipLinear,
		MinPointMagLinearMipPoint,
		MinPointMagLinearMipLinear,
		MinLinearMagPointMipPoint,
		MinLinearMagPointMipLinear,
		MinLinearMagLinearMipPoint,
		MinLinearMagLinearMipLinear,
		Anisotropy
	};
	
}