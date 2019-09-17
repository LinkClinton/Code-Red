#pragma once

#include <exception>

#undef throwIf

#ifdef _DEBUG
#define throwIf(condition, exception) if ((condition) == false) throw exception;
#else
#define throwIf(condition, exception) 
#endif

namespace CodeRed {
	
	class Exception : public std::exception {
	public:
		Exception() = default;

		explicit Exception(const char* const message) :
			std::exception(message) {}
	};
}