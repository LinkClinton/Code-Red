#pragma once

namespace CodeRed {

	using UInt64 = unsigned long long;
	using UInt32 = unsigned;
	using UInt8 = unsigned char;
	using Byte = unsigned char;
	using Real = float;

#undef CODE_RED_DEBUG_THROW_IF
#ifdef __ENABLE__CODE__RED__DEBUG__
#define CODE_RED_DEBUG_THROW_IF(condition, exception) if ((condition)) throw exception;
#else
#define CODE_RED_DEBUG_THROW_IF(condition, exception)
#endif
	
}