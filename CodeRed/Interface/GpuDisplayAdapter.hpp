#pragma once

#include "../Shared/Noncopyable.hpp"

#include <string>

namespace CodeRed {

	class GpuDisplayAdapter : public Noncopyable {
	protected:
		GpuDisplayAdapter() = default;
		
		~GpuDisplayAdapter() = default;
	public:
		virtual auto name() const->std::string = 0;
	};
	
}