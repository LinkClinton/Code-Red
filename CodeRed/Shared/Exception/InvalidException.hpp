#pragma once

#include "Exception.hpp"
#include "../DebugReport.hpp"

#include <typeinfo>
#include <vector>
#include <string>

namespace CodeRed {

	template<typename T>
	class InvalidException final : public Exception {
	public:
		explicit InvalidException(
			const std::vector<std::string>& name) :
			Exception(DebugReport::makeError(message_format,
				{ name.empty() ? "unnamed obejct" : name[0] })) {}

		explicit InvalidException(
			const std::vector<std::string>& name,
			const std::vector<std::string>& debug_message) :
			Exception(DebugReport::makeError(message_format, 
				{ name.empty() ? "unnamed object" : name[0] },
				debug_message)) {}
	private:
		const inline static std::string message_format = 
			std::string("The [0](type : ") + typeid(T).name() + ") is invalid.";
	};
}