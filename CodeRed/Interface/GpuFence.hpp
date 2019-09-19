#pragma once

#ifdef __CODE__RED__GLOBAL__INCLUDE__
#include <Shared/Noncopyable.hpp>
#else
#include "../Shared/Noncopyable.hpp"
#endif

#include <memory>

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