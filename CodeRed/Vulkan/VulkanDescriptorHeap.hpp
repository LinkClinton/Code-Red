#pragma once

#include "../Interface/GpuDescriptorHeap.hpp"
#include "VulkanUtility.hpp"

#ifdef __ENABLE__VULKAN__

namespace CodeRed {

	class VulkanDescriptorHeap final : public GpuDescriptorHeap {
	public:
		explicit VulkanDescriptorHeap(
			const std::shared_ptr<GpuLogicalDevice>& device,
			const std::shared_ptr<GpuResourceLayout>& layout);

		~VulkanDescriptorHeap();

		void bindTexture(
			const size_t index, 
			const std::shared_ptr<GpuTexture>& texture) override;

		void bindBuffer(
			const size_t index, 
			const std::shared_ptr<GpuBuffer>& buffer) override;

		auto descriptorSets() const noexcept -> std::vector<vk::DescriptorSet> { return mDescriptorSets; }
	private:
		std::vector<vk::DescriptorSet> mDescriptorSets;
		
		vk::DescriptorPool mDescriptorPool;
	};
	
}

#endif