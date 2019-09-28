#pragma once

#include "Exception.hpp"
#include "../DebugReport.hpp"

#include <typeinfo>
#include <vector>
#include <string>

namespace CodeRed {

	template<typename T>
	class InvalidException : public Exception {
	public:
		explicit InvalidException(
			const std::vector<std::string>& messages, T* value = nullptr) :
			InvalidException(messages, value, messageTemplate) {}

		explicit InvalidException(
			const std::vector<std::string>& messages,
			const std::string& debugMessage,
			T* value = nullptr) :
			InvalidException(messages, debugMessage, messageTemplate, value) {}
		
		auto value() const -> T* { return mValue; }
	protected:
		InvalidException(
			const std::vector<std::string> &messages, T* value,
			const std::string& message) :
			Exception(DebugReport::push(message, messages)),
			mValue(value) {}

		InvalidException(
			const std::vector<std::string>& messages,
			const std::string& debugMessage,
			const std::string& message,
			T* value) :
			Exception(DebugReport::push(message, messages) + "\nDebug Message : " + debugMessage),
			mValue(value) {}
	private:
		const std::string messageTemplate = std::string("The [0](type : ") + typeid(T).name() + ") is invalid.";
		
		T* mValue;
	};
}