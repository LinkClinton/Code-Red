#pragma once

#include <exception>
#include <string>

namespace CodeRed {
	
	class Exception : public std::exception {
	public:
		Exception() = default;

		explicit Exception(const std::string& message);
		
		char const* what() const override;
	private:
		std::string mMesaage;
	};
}