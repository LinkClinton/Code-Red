#pragma once

#include "Exception.hpp"
#include "../Enum/DebugType.hpp"

#include <string>
#include <vector>

namespace CodeRed {

	class FailedException final : public Exception {
	public:
		explicit FailedException(
			const DebugType type,
			const std::vector<std::string>& messages);

		explicit FailedException(
			const DebugType type,
			const std::vector<std::string>& messages,
			const std::vector<std::string>& debug_message);
	};
}
