#pragma once

#include "../Utility.hpp"

namespace CodeRed {

	enum class DebugType : UInt32
	{
		//Create [0] failed.
		Create,
		//Get [0] from [1] failed.
		Get,
		//Set [0] to [1] failed.
		Set
	};
	
}