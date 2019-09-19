#pragma once

#ifdef __CODE__RED__GLOBAL__INCLUDE__
#include <Shared/Exception/Exception.hpp>
#include <Shared/DebugReport.hpp>
#else
#include "Exception.hpp"
#include "../DebugReport.hpp"
#endif

#include <string>
#include <vector>

namespace CodeRed {

	class FailedException : public Exception {
	public:
		using Type = DebugType;
	public:
		explicit FailedException(
			const std::vector<std::string> &objects, 
			const Type type = Type::Create) :
			Exception(DebugReport::push(DebugReport::select(type), objects)) {}
	};
}
