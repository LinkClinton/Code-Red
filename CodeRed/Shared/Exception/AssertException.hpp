#pragma once

#include <Shared/Exception/Exception.hpp>

#undef assert

#ifdef _DEBUG
#define assert(condition) if (!(condition)) throw AssertException();
#else
#define assert(condition) 
#endif

namespace CodeRed {
	
	class AssertException : public Exception {
	public:
		AssertException() :
			Exception("Assert Exception.") {}
	};
	
}