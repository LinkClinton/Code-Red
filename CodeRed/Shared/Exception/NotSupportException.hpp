#pragma once

#include "Exception.hpp"
#include "../Utility.hpp"

namespace CodeRed {

	enum class NotSupportType : UInt32 {
		Any,
		Method,
		Object,
		Enum
	};
	
	class NotSupportException final : public Exception {
	public:
		explicit NotSupportException(
			const NotSupportType type = NotSupportType::Any);
	private:
		static auto select(const NotSupportType type) -> std::string;
	};
	
}