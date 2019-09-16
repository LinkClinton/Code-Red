#pragma once

#include "../Shared/Noncopyable.hpp"

#include <memory>

namespace CodeRed {

	class GpuGraphicsCommandList;
	class GpuLogicalDevice;
	
	class GpuCommandAllocator : public Noncopyable {
	protected:
		explicit GpuCommandAllocator(
			const std::shared_ptr<GpuLogicalDevice> &device) :
			mDevice(device) {}

		~GpuCommandAllocator() = default;

		virtual void reset() = 0;
	protected:
		std::shared_ptr<GpuLogicalDevice> mDevice;
	};
	
}