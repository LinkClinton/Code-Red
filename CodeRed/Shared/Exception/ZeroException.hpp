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
			InvalidException<T>(messages, value, messageTemplate) {}

		explicit ZeroException(
			const std::vector<std::string> &messages,
			const std::string& debugMessage,
			T* value = nullptr) :
			InvalidException<T>(messages, debugMessage, messageTemplate, value) {}
	private:
		const std::string messageTemplate = std::string("The [0](type : ") + typeid(T).name() + ") is zero/nullptr.";
	};
	
}