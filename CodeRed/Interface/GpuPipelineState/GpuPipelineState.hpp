#pragma once

#ifdef __CODE__RED__GLOBAL__INCLUDE__
#include <Shared/Noncopyable.hpp>
#else
#include "../../Shared/Noncopyable.hpp"
#endif

#include <memory>

namespace CodeRed {

	class GpuLogicalDevice;
	
	class GpuPipelineState : public Noncopyable {
	protected:
		explicit GpuPipelineState(
			const std::shared_ptr<GpuLogicalDevice> &device) :
			mDevice(device) {}
		
		virtual ~GpuPipelineState() = default;
	protected:
		std::shared_ptr<GpuLogicalDevice> mDevice;
	};
	
}