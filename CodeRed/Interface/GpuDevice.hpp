#pragma once

#include "../Shared/Noncopyable.hpp"

#include <memory>

namespace CodeRed {

	class GpuDevice : public Noncopyable {
	public:
		~GpuDevice() = default;
	};
	
}