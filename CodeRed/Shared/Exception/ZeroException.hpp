#pragma once

#ifdef __CODE__RED__GLOBAL__INCLUDE__
#include <Shared/Exception/InvalidException.hpp>
#else
#include "InvalidException.hpp"
#endif

namespace CodeRed {

	template<typename T>
	class ZeroException : public InvalidException<T> {
	public:
		explicit ZeroException(
			const std::vector<std::string> &messages,
			T* value = nullptr) :
			InvalidException<T>(messages, value, "The [0] is zero.") {}
	};
	
}