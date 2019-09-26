#pragma once

#ifdef __CODE__RED__GLOBAL__INCLUDE__
#include <Shared/LayoutElement.hpp>
#else
#include "../Shared/LayoutElement.hpp"
#endif

#include <memory>
#include <vector>

namespace CodeRed {

	class GpuLogicalDevice;
	class GpuResource;
	class GpuTexture;
	class GpuBuffer;
	
	class GpuResourceLayout {
	protected:
		explicit GpuResourceLayout(
			const std::shared_ptr<GpuLogicalDevice>& device,
			const std::vector<ResourceLayoutElement>& elements,
			const std::vector<SamplerLayoutElement>& samplers,
			const size_t maxBindResources = 1 << 10);
		
		~GpuResourceLayout() = default;
	public:
		auto element(const size_t index) const -> ResourceLayoutElement { return mElements[index]; }

		auto sampler(const size_t index) const -> SamplerLayoutElement { return mSamplers[index]; }
		
		auto elements() const noexcept -> std::vector<ResourceLayoutElement> { return mElements; }

		auto samplers() const noexcept -> std::vector<SamplerLayoutElement> { return mSamplers; }

		auto maxBindResources() const noexcept -> size_t { return mMaxBindResources; }
		
		void bindResource(
			const size_t index,
			const std::shared_ptr<GpuResource>& resource);

		virtual void reset() = 0;
		
		virtual void bindTexture(
			const size_t index,
			const std::shared_ptr<GpuTexture>& resource) = 0;
		
		virtual void bindBuffer(
			const size_t index,
			const std::shared_ptr<GpuBuffer>& resource) = 0;

		virtual void unbindResource(
			const std::shared_ptr<GpuResource>& resource) = 0;
	protected:
		std::shared_ptr<GpuLogicalDevice> mDevice;
		
		std::vector<ResourceLayoutElement> mElements = {};
		std::vector<SamplerLayoutElement> mSamplers = {};

		size_t mMaxBindResources = 0;
	};
	
}