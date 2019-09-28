#pragma once

#include "Exception.hpp"
#include "../Utility.hpp"

namespace CodeRed {

	enum class NotSupportType : UInt32 {
		Any,
		Method,
		Object,
		Enum
	};
	
	class NotSupportException : public Exception {
	public:
		using Type = NotSupportType;
	public:
		NotSupportException(const Type type = Type::Any);
	private:
		static auto select(const Type type) -> const char*;
	};
	
}