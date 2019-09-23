#pragma once

#ifdef __CODE__RED__GLOBAL__INCLUDE__
#include <Shared/Exception/Exception.hpp>
#include <Shared/Enum/DebugType.hpp>
#else
#include "Exception.hpp"
#include "../Enum/DebugType.hpp"
#endif

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
