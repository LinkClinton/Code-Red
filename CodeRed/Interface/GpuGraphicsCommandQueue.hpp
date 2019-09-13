#pragma once

#include "../Shared/Noncopyable.hpp"

#include <memory>
#include <vector>

namespace CodeRed {

	class GpuGraphicsCommandList;
	class GpuLogicDevice;
	
	class GpuGraphicsCommandQueue : public Noncopyable {
	protected:
		GpuGraphicsCommandQueue(
			const std::shared_ptr<GpuLogicDevice>& device) :
			mDevice(device) {}

		~GpuGraphicsCommandQueue() = default;
	public:
		virtual void execute(const std::vector<std::shared_ptr<GpuGraphicsCommandList>>& lists) = 0;
	protected:
		std::shared_ptr<GpuLogicDevice> mDevice;
	};
	
}