#pragma once

#include "InvalidException.hpp"

namespace CodeRed {

	template<typename T>
	class ZeroException final : public Exception {
	public:
		explicit ZeroException(
			const std::vector<std::string>& name) :
			Exception(DebugReport::makeError(message_format, 
				{ name.empty() ? "unnamed obejct" : name[0] })) {}

		explicit ZeroException(
			const std::vector<std::string>& name,
			const std::vector<std::string>& debug_message) :
			Exception(DebugReport::makeError(message_format,
				{ name.empty() ? "unnamed object" : name[0] },
				debug_message)) {}
	private:
		const inline static std::string message_format
			= std::string("The [0](type : ") + typeid(T).name() + ") is zero/nullptr.";
	};
	
}