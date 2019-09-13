#pragma once

#include "Exception.hpp"

namespace CodeRed {

	template<typename T>
	class InvalidException : public Exception {
	public:
		InvalidException(T* value = nullptr) :
			InvalidException(value, "The value is invalid.") {}

		auto value() const -> T* { return mValue; }
	protected:
		InvalidException(T* value, const char* const message) :
			Exception(message),
			mValue(value) {}
	private:
		T* mValue;
	};
}