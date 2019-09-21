#pragma once

#include <Core/CodeRedGraphics.hpp>

namespace CodeRed {

	class ResourceHelper {
	public:
		static void updateBuffer(
			const std::shared_ptr<GpuLogicalDevice>& device,
			const std::shared_ptr<GpuCommandAllocator> &allocator,
			const std::shared_ptr<GpuBuffer>& buffer,
			const void* data
		);
	};
	
}