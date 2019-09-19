#pragma once

#include <Shared/Noncopyable.hpp>

#include <memory>
#include <vector>

namespace CodeRed {

	class GpuLogicalDevice;
	class GpuCommandQueue;
	
	class GpuFence : public Noncopyable {
	protected:
		explicit GpuFence(
			const std::shared_ptr<GpuLogicalDevice>& device);
		
		~GpuFence() = default;
	protected:
		std::shared_ptr<GpuLogicalDevice> mDevice;
	};
	
}