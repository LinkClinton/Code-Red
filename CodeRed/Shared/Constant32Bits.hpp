#pragma once

#include "Enum/ShaderVisibility.hpp"
#include "Utility.hpp"

#include <variant>

namespace CodeRed {

	struct Value32Bit {
		union {
			UInt32 UInt;
			Int32 Int;
			Real Float;
		};

		Value32Bit(const UInt32 uInt32) : UInt(uInt32) {}

		Value32Bit(const Int32 int32) : Int(int32) {}

		Value32Bit(const Real real) : Float(real) {}
	};
	
	struct Constant32Bits {
		ShaderVisibility Visibility = ShaderVisibility::All;
		size_t Binding = 0;
		size_t Space = 0;
		size_t Count = 1;

		Constant32Bits() = default;

		explicit Constant32Bits(
			const size_t binding,
			const size_t count = 1,
			const size_t space = 0,
			const ShaderVisibility visibility = ShaderVisibility::All) :
			Visibility(visibility),
			Binding(binding),
			Space(space),
			Count(count) {}
	};
	
}