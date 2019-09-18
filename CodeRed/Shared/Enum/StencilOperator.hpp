#pragma once

#include <Shared/Utility.hpp>

namespace CodeRed {

	enum class StencilOperator : UInt32
	{
		Keep,
		Zero,
		Replace,
		IncrementAndClamp,
		DecrementAndClamp,
		Invert,
		IncrementAndWrap,
		DecrementAndWrap
	};
	
}