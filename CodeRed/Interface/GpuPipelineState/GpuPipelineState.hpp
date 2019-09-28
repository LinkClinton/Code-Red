#pragma once

#include "../../Shared/Noncopyable.hpp"

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