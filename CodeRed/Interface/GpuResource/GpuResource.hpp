#pragma once

#include "../../Shared/Information/ResourceInfo.hpp"
#include "../../Shared/Noncopyable.hpp"

#include <memory>

namespace CodeRed {

	class GpuLogicalDevice;
	
	class GpuResource : public Noncopyable {
	protected:
		explicit GpuResource(
			const std::shared_ptr<GpuLogicalDevice>& device,
			const ResourceInfo& resource_info) :
			mDevice(device),
			mResourceInfo(resource_info) {}
		
		~GpuResource() = default;
	public:
		auto info() const noexcept -> ResourceInfo { return mResourceInfo; }

		auto usage() const noexcept -> ResourceUsage { return mResourceInfo.Usage; }

		auto layout() const noexcept -> ResourceLayout { return mResourceInfo.Layout; }
		
		auto heap() const noexcept -> MemoryHeap { return mResourceInfo.Heap; }
		
		virtual auto size() const -> size_t = 0;

		virtual auto mapMemory() const -> void* = 0;

		virtual void unmapMemory() const = 0;
	protected:
		std::shared_ptr<GpuLogicalDevice> mDevice;
		
		ResourceInfo mResourceInfo;
	};

}