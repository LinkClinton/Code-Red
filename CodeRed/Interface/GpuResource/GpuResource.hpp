#pragma once

#include "../../Shared/Information/ResourceInfo.hpp"
#include "../../Shared/Noncopyable.hpp"

#include <memory>

namespace CodeRed {

	class GpuLogicalDevice;
	
	class GpuResource : public Noncopyable, public std::enable_shared_from_this<GpuResource> {
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

		auto type() const noexcept -> ResourceType { return mInfo.Type; }
		
		auto layout() const noexcept -> ResourceLayout { return mInfo.Layout; }
		
		auto heap() const noexcept -> MemoryHeap { return mInfo.Heap; }

		virtual auto size() const -> size_t = 0;
	protected:
		friend class DirectX12GraphicsCommandList;
		friend class VulkanGraphicsCommandList;
		
		void setLayout(const ResourceLayout layout) { mInfo.Layout = layout; }
	protected:
		std::shared_ptr<GpuLogicalDevice> mDevice;
		
		ResourceInfo mInfo;
	};

}