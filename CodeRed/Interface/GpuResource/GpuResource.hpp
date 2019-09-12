#pragma once

#include "../../Shared/ResourceInfo.hpp"
#include "../../Shared/Noncopyable.hpp"

#include <memory>

namespace CodeRed {

	class GpuLogicalDevice;
	
	class GpuResource : public Noncopyable {
	public:
		GpuResource(
			const std::shared_ptr<GpuLogicalDevice>& device,
			const ResourceInfo& resource_info) :
			mDevice(device),
			mResourceInfo(resource_info) {}
		
		~GpuResource() = default;

		virtual auto mapMemory() const -> void* = 0;

		virtual void unmapMemory() const = 0;
	protected:
		std::shared_ptr<GpuLogicalDevice> mDevice;
		
		ResourceInfo mResourceInfo;
	};

}