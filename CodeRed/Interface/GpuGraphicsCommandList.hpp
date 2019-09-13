#pragma once

#include "../Shared/Noncopyable.hpp"

#include <memory>

namespace CodeRed {

	class GpuLogicalDevice;
	
	class GpuGraphicsCommandList : public Noncopyable {
	protected:
		GpuGraphicsCommandList(const std::shared_ptr<GpuLogicalDevice>& device) {}
		
		~GpuGraphicsCommandList() = default;
	};
	
}