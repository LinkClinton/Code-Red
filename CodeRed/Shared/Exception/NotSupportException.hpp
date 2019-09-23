#pragma once

#ifdef __CODE__RED__GLOBAL__INCLUDE__
#include <Shared/Exception/Exception.hpp>
#include <Shared/Utility.hpp>
#else
#include "Exception.hpp"
#include "../Utility.hpp"
#endif

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