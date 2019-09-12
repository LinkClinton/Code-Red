#pragma once

#include "InvalidException.hpp"

namespace CodeRed {

	template<typename T>
	class ZeroException : public InvalidException<T> {
	public:
		ZeroException(T* value = nullptr) :
			InvalidException<T>(value, "The value is zeor.") {}
	};
	
}