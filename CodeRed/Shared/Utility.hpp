#pragma once

namespace CodeRed {

	using UInt64 = unsigned long long;
	using UInt32 = unsigned;
	using UInt8 = unsigned char;
	using Byte = unsigned char;
	using Real = float;


#undef CODE_RED_DEBUG_THROW_IF
#undef CODE_RED_TRY_EXECUTE
#undef CODE_RED_DEBUG_TRY_EXECUTE
#undef CODE_RED_DEBUG_TO_STRING
	
#define CODE_RED_TRY_EXECUTE(condition, expression) if (condition) expression;
#define CODE_RED_TO_STRING(value) #value
	
#ifdef __ENABLE__CODE__RED__DEBUG__
#define CODE_RED_DEBUG_THROW_IF(condition, exception) if ((condition)) throw exception;
#define CODE_RED_DEBUG_TRY_EXECUTE(condition, expression) if (condition) expression;
#else
#define CODE_RED_DEBUG_THROW_IF(condition, exception)
#define CODE_RED_DEBUG_TRY_EXECUTE(condition, expression)
#endif
	
}