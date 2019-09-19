#pragma once

#ifdef __CODE__RED__GLOBAL__INCLUDE__
#include <Shared/Enum/CompareOperator.hpp>
#include <Shared/Enum/StencilOperator.hpp>
#else
#include "Enum/CompareOperator.hpp"
#include "Enum/StencilOperator.hpp"
#endif

namespace CodeRed {

	struct StencilOperatorInfo {
		CompareOperator CompareOperator = CompareOperator::Always;
		StencilOperator FailOperator = StencilOperator::Keep;
		StencilOperator PassOperator = StencilOperator::Keep;

		StencilOperator DepthFailOperator = StencilOperator::Keep;

		StencilOperatorInfo() = default;
	};
	
}