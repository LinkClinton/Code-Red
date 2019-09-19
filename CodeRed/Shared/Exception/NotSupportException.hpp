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
		NotSupportException(const Type type = Type::Any) :
			Exception(select(type)) {}
	private:
		static auto select(const Type type) -> const char* {
			switch (type) {
			case Type::Any: return "We do not support this.";
			case Type::Method: return "We do not support this method.";
			case Type::Object: return "We do not support this object";
			case Type::Enum: return "We do not support this enum.";
			default:
				throw NotSupportException(Type::Enum);
			}
		}
	};
	
}