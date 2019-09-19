#pragma once

#ifdef __CODE__RED__GLOBAL__INCLUDE__
#include <Shared/Exception/Exception.hpp>
#else
#include "Exception.hpp"
#endif

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