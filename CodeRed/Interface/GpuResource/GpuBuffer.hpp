#pragma once

#include "GpuResource.hpp"

namespace CodeRed {

	class GpuBuffer : public GpuResource {
	protected:
		GpuBuffer(
			const std::shared_ptr<GpuLogicalDevice> &device,
			const ResourceInfo &resource_info) :
			GpuResource(device, resource_info) {}

		~GpuBuffer() = default;
	public:
		auto size() const -> size_t override { return std::get<BufferProperty>(mResourceInfo.Property).Size; }

		auto stride() const -> size_t { return std::get<BufferProperty>(mResourceInfo.Property).Stride; }

		auto count() const -> size_t { return std::get<BufferProperty>(mResourceInfo.Property).Count; }
	};
	
}