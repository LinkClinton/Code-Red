#pragma once

#ifdef __CODE__RED__GLOBAL__INCLUDE__
#include <Shared/Noncopyable.hpp>
#else
#include "../Shared/Noncopyable.hpp"
#endif

#include <memory>
#include <vector>

namespace CodeRed {

	class GpuGraphicsCommandList;
	class GpuLogicalDevice;
	
	class GpuCommandQueue : public Noncopyable {
	protected:
		explicit GpuCommandQueue(
			const std::shared_ptr<GpuLogicalDevice>& device);
		
		~GpuCommandQueue() = default;
	public:
		virtual void execute(const std::vector<std::shared_ptr<GpuGraphicsCommandList>>& lists) = 0;

		virtual void waitIdle() = 0;
	protected:
		std::shared_ptr<GpuLogicalDevice> mDevice;
	};
	
}