#pragma once

#include "../Shared/DebugReport.hpp"
#include "../Utility.hpp"
#include "Exception.hpp"

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
