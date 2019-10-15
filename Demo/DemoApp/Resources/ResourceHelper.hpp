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

		static void updateBuffer(
			const std::shared_ptr<GpuLogicalDevice>& device,
			const std::shared_ptr<GpuCommandAllocator>& allocator,
			const std::shared_ptr<GpuCommandQueue>& queue,
			const std::shared_ptr<GpuBuffer>& buffer,
			const void* data
		);

		static void updateBuffer(
			const std::shared_ptr<GpuBuffer>& buffer,
			const void* data,
			const size_t size = 0);

		static void updateTexture(
			const std::shared_ptr<GpuLogicalDevice>& device,
			const std::shared_ptr<GpuCommandAllocator>& allocator,
			const std::shared_ptr<GpuCommandQueue>& queue,
			const std::shared_ptr<GpuTexture>& texture,
			const void* data
		);
		
		static auto loadTexture(
			const std::shared_ptr<GpuLogicalDevice>& device,
			const std::shared_ptr<GpuCommandAllocator>& allocator,
			const std::shared_ptr<GpuCommandQueue>& queue,
			const std::string& fileName
		) -> std::shared_ptr<GpuTexture>;
	private:
		static auto formatMapped(int channel) -> PixelFormat;
	};
	
}