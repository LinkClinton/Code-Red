#pragma once

#include "../Shared/Noncopyable.hpp"

#include <memory>
#include <vector>

namespace CodeRed {

	class GpuGraphicsCommandList;
	class GpuLogicalDevice;
	class GpuResource;
	
	class GpuCommandAllocator : public Noncopyable {
	protected:
		explicit GpuCommandAllocator(
			const std::shared_ptr<GpuLogicalDevice>& device);
		
		~GpuCommandAllocator() = default;
	public:
		virtual void reset() = 0;
	protected:
		std::shared_ptr<GpuLogicalDevice> mDevice;

		std::vector<std::shared_ptr<GpuResource>> mCopyCacheResources;
	};
	
}