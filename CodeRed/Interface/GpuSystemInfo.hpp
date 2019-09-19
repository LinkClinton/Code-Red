#pragma once

#ifdef __CODE__RED__GLOBAL__INCLUDE__
#include <Shared/Noncopyable.hpp>
#else
#include "../Shared/Noncopyable.hpp"
#endif

#include <memory>
#include <vector>

namespace CodeRed {

	class GpuDisplayAdapter;
	
	class GpuSystemInfo : public Noncopyable {
	protected:
		GpuSystemInfo() = default;
		
		~GpuSystemInfo() = default;
	public:
		virtual auto selectDisplayAdapter() const->std::vector<std::shared_ptr<GpuDisplayAdapter>> = 0;
	};
	
}