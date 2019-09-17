#pragma once

#include "../Shared/LayoutElement.hpp"

#include <memory>
#include <vector>

namespace CodeRed {

	class GpuLogicalDevice;
	
	class GpuResourceLayout {
	protected:
		explicit GpuResourceLayout(
			const std::shared_ptr<GpuLogicalDevice> &device,
			const std::vector<ResourceLayoutElement>& elements,
			const std::vector<SamplerLayoutElement>& samplers)
			: mDevice(device), mElements(elements), mSamplers(samplers) {}
		
		~GpuResourceLayout() = default;
	public:
		auto element(const size_t index) const -> ResourceLayoutElement { return mElements[index]; }

		auto sampler(const size_t index) const -> SamplerLayoutElement { return mSamplers[index]; }
		
		auto elements() const noexcept -> std::vector<ResourceLayoutElement> { return mElements; }

		auto samplers() const noexcept -> std::vector<SamplerLayoutElement> { return mSamplers; }
	protected:
		std::shared_ptr<GpuLogicalDevice> mDevice;
		
		std::vector<ResourceLayoutElement> mElements = {};
		std::vector<SamplerLayoutElement> mSamplers = {};
	};
	
}