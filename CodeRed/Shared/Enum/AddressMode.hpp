#pragma once

#include <Shared/Utility.hpp>

namespace CodeRed {

	enum class AddressMode : UInt32
	{
		Repeat,
		Mirror,
		Clamp,
		Border
	};
	
}