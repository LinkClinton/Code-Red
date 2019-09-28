#pragma once

#include "Utility.hpp"

namespace CodeRed {

	struct ClearValue {
		Real Red = 1.0f;
		Real Green = 1.0f;
		Real Blue = 1.0f;
		Real Alpha = 1.0f;
		Real Depth = 0.0f;
		
		UInt8 Stencil = 0;

		ClearValue() = default;

		ClearValue(
			const Real red,
			const Real green,
			const Real blue,
			const Real alpha) :
			Red(red), Green(green), Blue(blue), Alpha(alpha) {}

		ClearValue(
			const Real depth,
			const UInt8 stencil) :
			Depth(depth), Stencil(stencil) {}
	};
	
}