#pragma once

#include "../Shared/LayoutElement.hpp"
#include "../Shared/Noncopyable.hpp"

#include <memory>
#include <vector>

namespace CodeRed {

	class GpuLogicalDevice;
	class GpuResource;
	class GpuTexture;
	class GpuBuffer;
	
	class GpuResourceLayout : public Noncopyable {
	protected:
		explicit GpuResourceLayout(
			const std::shared_ptr<GpuLogicalDevice>& device,
			const std::vector<ResourceLayoutElement>& elements,
			const std::vector<SamplerLayoutElement>& samplers);
		
		~GpuResourceLayout() = default;
	public:
		auto element(const size_t index) const -> ResourceLayoutElement { return mElements[index]; }

		auto sampler(const size_t index) const -> SamplerLayoutElement { return mSamplers[index]; }
		
		auto elements() const noexcept -> std::vector<ResourceLayoutElement> { return mElements; }

		auto samplers() const noexcept -> std::vector<SamplerLayoutElement> { return mSamplers; }
	protected:
		friend class DirectX12DescriptorHeap;
		friend class VulkanDescriptorHeap;
		
		std::shared_ptr<GpuLogicalDevice> mDevice;
		
		std::vector<ResourceLayoutElement> mElements = {};
		std::vector<SamplerLayoutElement> mSamplers = {};
	};
	
}