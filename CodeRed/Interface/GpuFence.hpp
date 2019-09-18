#pragma once

#include <Shared/Noncopyable.hpp>

#include <memory>
#include <vector>

namespace CodeRed {

	class GpuLogicalDevice;
	class GpuGraphicsCommandQueue;
	
	class GpuFence : public Noncopyable {
	protected:
		explicit GpuFence(
			const std::shared_ptr<GpuLogicalDevice>& device) :
			mDevice(device) {}
		
		~GpuFence() = default;
	public:
		virtual void wait(
			const std::vector<std::shared_ptr<GpuGraphicsCommandQueue>>& queues) = 0;
	protected:
		std::shared_ptr<GpuLogicalDevice> mDevice;
	};
	
}