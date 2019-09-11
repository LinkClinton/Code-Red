#pragma once

#include "../Shared/Noncopyable.hpp"

#include <string>

namespace CodeRed {

	class GpuDisplayAdapter : public Noncopyable {
	public:
		~GpuDisplayAdapter() = default;

		virtual auto name() const->std::string = 0;
	};
	
}