#pragma once

#ifdef __CODE__RED__GLOBAL__INCLUDE__
#include <Shared/Information/ResourceInfo.hpp>
#include <Shared/Noncopyable.hpp>
#else
#include "../../Shared/Information/ResourceInfo.hpp"
#include "../../Shared/Noncopyable.hpp"
#endif

#include <memory>

namespace CodeRed {

	class GpuLogicalDevice;
	
	class GpuResource : public Noncopyable {
	protected:
		explicit GpuResource(
			const std::shared_ptr<GpuLogicalDevice>& device,
			const ResourceInfo& info) :
			mDevice(device),
			mInfo(info) {}
		
		~GpuResource() = default;
	public:
		auto info() const noexcept -> ResourceInfo { return mInfo; }

		auto usage() const noexcept -> ResourceUsage { return mInfo.Usage; }

		auto layout() const noexcept -> ResourceLayout { return mInfo.Layout; }
		
		auto heap() const noexcept -> MemoryHeap { return mInfo.Heap; }
		
		virtual auto size() const -> size_t = 0;

		virtual auto mapMemory() const -> void* = 0;

		virtual void unmapMemory() const = 0;
	protected:
		std::shared_ptr<GpuLogicalDevice> mDevice;
		
		ResourceInfo mInfo;
	};

}