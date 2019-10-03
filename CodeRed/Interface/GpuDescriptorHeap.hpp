#pragma once

#include "../Shared/Noncopyable.hpp"

#include <memory>

namespace CodeRed {

	class GpuResource;
	class GpuTexture;
	class GpuBuffer;

	class GpuLogicalDevice;
	class GpuResourceLayout;
	
	class GpuDescriptorHeap : public Noncopyable {
	protected:
		explicit GpuDescriptorHeap(
			const std::shared_ptr<GpuLogicalDevice>& device,
			const std::shared_ptr<GpuResourceLayout>& resource_layout);

		~GpuDescriptorHeap() = default;
	public:
		void bindResource(
			const std::shared_ptr<GpuResource>& resource,
			const size_t index);

		virtual void bindTexture(
			const std::shared_ptr<GpuTexture>& texture,
			const size_t index) = 0;

		virtual void bindBuffer(
			const std::shared_ptr<GpuBuffer>& buffer,
			const size_t index) = 0;

		auto count() const noexcept -> size_t { return mCount; }
		
		auto layout() const noexcept -> std::shared_ptr<GpuResourceLayout> { return mResourceLayout; }
	protected:
		std::shared_ptr<GpuResourceLayout> mResourceLayout;
		std::shared_ptr<GpuLogicalDevice> mDevice;

		size_t mCount = 0;
	};
	
}