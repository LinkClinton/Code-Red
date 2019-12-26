#pragma once

namespace CodeRed {

	template<typename T>
	struct ValueRange {
		T Start = T();
		T End = T();

		ValueRange() = default;

		ValueRange(const T& start, const T& end) :
			Start(start), End(end) {}

		auto size() const noexcept -> T { return End - Start; }
	};
	
}