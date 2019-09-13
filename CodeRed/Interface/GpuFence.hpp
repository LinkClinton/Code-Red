#pragma once

#include "../Shared/Noncopyable.hpp"

#include <memory>

namespace CodeRed {

	class GpuLogicalDevice;
	
	class GpuFence : public Noncopyable {
	protected:
		GpuFence(
			const std::shared_ptr<GpuLogicalDevice>& device) {}
		
		~GpuFence() = default;
	};
	
}