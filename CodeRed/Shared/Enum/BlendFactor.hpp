#pragma once

#include "../Utility.hpp"

namespace CodeRed {

	enum class BlendFactor : UInt32
	{
		Zero,
		One,
		SrcColor,
		InvSrcColor,
		SrcAlpha,
		InvSrcAlpha,
		DestAlpha,
		InvDestAlpha,
		DescColor,
		InvDestColor,
		Factor,
		InvFactor
	};
	
}