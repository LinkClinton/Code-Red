#pragma once

#include "../../Shared/ResourceInfo.hpp"
#include "../../Shared/Noncopyable.hpp"

#include <memory>

namespace CodeRed {

	class GpuLogicalDevice;
	
	class GpuResource : public Noncopyable {
	protected:
		GpuResource(
			const std::shared_ptr<GpuLogicalDevice>& device,
			const ResourceInfo& resource_info) :
			mDevice(device),
			mResourceInfo(resource_info) {}
		
		~GpuResource() = default;
	public:
		auto info() const -> ResourceInfo { return mResourceInfo; }

		auto usage() const -> ResourceUsage { return mResourceInfo.Usage; }

		auto heap() const -> MemoryHeap { return mResourceInfo.Heap; }
		
		virtual auto size() const -> size_t = 0;
		
		virtual auto mapMemory() const -> void* = 0;

		virtual void unmapMemory() const = 0;
	protected:
		std::shared_ptr<GpuLogicalDevice> mDevice;
		
		ResourceInfo mResourceInfo;
	};

}