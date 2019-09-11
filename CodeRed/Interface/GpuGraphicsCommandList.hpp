#pragma once

#include "../Shared/Noncopyable.hpp"

#include <memory>

namespace CodeRed {

	class GpuLogicalDevice;
	
	class GpuGraphicsCommandList : public Noncopyable {
	public:
		GpuGraphicsCommandList(const std::shared_ptr<GpuLogicalDevice>& device) {}
		
		~GpuGraphicsCommandList() = default;

		
	};
	
}