#pragma once

#include "Enum/MultiSample.hpp"

namespace CodeRed {

	class MultiSampleSizeOf {
	public:
		MultiSampleSizeOf() = default;

		static auto get(const MultiSample sample) -> size_t;
	};
	
}
