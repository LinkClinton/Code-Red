#pragma once

#include "Exception.hpp"
#include "../Enum/DebugType.hpp"

#include <string>
#include <vector>

namespace CodeRed {

	class FailedException : public Exception {
	public:
		using Type = DebugType;
	public:
		explicit FailedException(
			const std::vector<std::string>& messages,
			const Type type = Type::Create);

		explicit FailedException(
			const std::vector<std::string>& messages,
			const std::string& debugMessage,
			const Type type = Type::Create);
	};
}
