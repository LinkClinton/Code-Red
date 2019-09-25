#pragma once

#include "../Interface/GpuResourceLayout.hpp"
#include "VulkanUtility.hpp"

#ifdef __ENABLE__VULKAN__

namespace CodeRed {

	class VulkanResourceLayout final : public GpuResourceLayout {
	public:
		explicit VulkanResourceLayout(
			const std::shared_ptr<GpuLogicalDevice>& device,
			const std::vector<ResourceLayoutElement>& elements,
			const std::vector<SamplerLayoutElement>& samplers);

		~VulkanResourceLayout();

		auto layout() const noexcept -> vk::PipelineLayout { return mPipelineLayout; }

		auto pool() const noexcept -> vk::DescriptorPool { return mDescriptorPool; }
	private:
		vk::PipelineLayout mPipelineLayout;
		vk::DescriptorPool mDescriptorPool;

		std::vector<vk::DescriptorSetLayout> mDescriptorSetLayouts;
		std::vector<vk::WriteDescriptorSet> mWriteDescriptorSets;
		std::vector<vk::DescriptorSet> mDescriptorSets;
		std::vector<size_t> mDescriptorBindings;
	};

}

#endif