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
			Exception((
				mMessage = DebugReport::push(DebugReport::select(type), objects)).c_str()) {}
	protected:
		std::string mMessage;
	};
}
