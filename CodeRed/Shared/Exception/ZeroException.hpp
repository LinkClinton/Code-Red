#pragma once

#include <Shared/Exception/InvalidException.hpp>

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