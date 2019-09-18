#pragma once

#include <Shared/Enum/CompareOperator.hpp>
#include <Shared/Enum/StencilOperator.hpp>

namespace CodeRed {

	struct StencilOperatorInfo {
		CompareOperator CompareOperator = CompareOperator::Always;
		StencilOperator FailOperator = StencilOperator::Keep;
		StencilOperator PassOperator = StencilOperator::Keep;

		StencilOperator DepthFailOperator = StencilOperator::Keep;

		StencilOperatorInfo() = default;
	};
	
}