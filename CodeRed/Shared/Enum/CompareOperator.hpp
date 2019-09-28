#pragma once

#include "../Utility.hpp"

namespace CodeRed {

	enum class CompareOperator : UInt32
	{
		Never,
		Less,
		Equal,
		LessEqual,
		Greater,
		NotEqual,
		GreaterEqual,
		Always
	};
	
}