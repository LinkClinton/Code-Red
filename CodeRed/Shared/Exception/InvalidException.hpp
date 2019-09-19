#pragma once

#ifdef __CODE__RED__GLOBAL__INCLUDE__
#include <Shared/Exception/Exception.hpp>
#include <Shared/DebugReport.hpp>
#else
#include "Exception.hpp"
#include "../DebugReport.hpp"
#endif

#include <vector>
#include <string>

namespace CodeRed {

	template<typename T>
	class InvalidException : public Exception {
	public:
		explicit InvalidException(
			const std::vector<std::string> &messages, T* value = nullptr) :
			InvalidException(messages, value, "The [0] is invalid.") {}

		auto value() const -> T* { return mValue; }
	protected:
		InvalidException(
			const std::vector<std::string> &messages, T* value, 
			const char* const message) :
			Exception((mMessage = DebugReport::push(message, messages)).c_str()),
			mValue(value) {}
	private:
		std::string mMessage;
		
		T* mValue;
	};
}