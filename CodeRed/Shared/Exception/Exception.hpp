#pragma once

#include <exception>
#include <string>

#undef throwIf

#ifdef _DEBUG
#define throwIf(condition, exception) if ((condition) == true) throw exception;
#else
#define throwIf(condition, exception) 
#endif

namespace CodeRed {
	
	class Exception : public std::exception {
	public:
		Exception() = default;

		explicit Exception(const std::string &message) :
			mMesaage(message) {}

		char const* what() const override { return mMesaage.c_str(); }
	private:
		std::string mMesaage;
	};
}