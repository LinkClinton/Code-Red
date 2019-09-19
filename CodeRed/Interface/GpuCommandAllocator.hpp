#pragma once

#ifdef __CODE__RED__GLOBAL__INCLUDE__
#include <Shared/Noncopyable.hpp>
#else
#include "../Shared/Noncopyable.hpp"
#endif

#include <memory>

namespace CodeRed {

	class GpuGraphicsCommandList;
	class GpuLogicalDevice;
	
	class GpuCommandAllocator : public Noncopyable {
	protected:
		explicit GpuCommandAllocator(
			const std::shared_ptr<GpuLogicalDevice>& device);
		
		~GpuCommandAllocator() = default;

		virtual void reset() = 0;
	protected:
		std::shared_ptr<GpuLogicalDevice> mDevice;
	};
	
}