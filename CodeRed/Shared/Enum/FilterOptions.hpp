#pragma once

#ifdef __CODE__RED__GLOBAL__INCLUDE__
#include <Shared/Utility.hpp>
#else
#include "../Utility.hpp"
#endif

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