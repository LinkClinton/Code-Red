#pragma once

#include "../Interface/GpuResourceLayout.hpp"
#include "../Shared/IdentityAllocator.hpp"
#include "VulkanUtility.hpp"

#include <map>

#ifdef __ENABLE__VULKAN__

namespace CodeRed {

	class VulkanResourceLayout final : public GpuResourceLayout {
	public:
		explicit VulkanResourceLayout(
			const std::shared_ptr<GpuLogicalDevice>& device,
			const std::vector<ResourceLayoutElement>& elements,
			const std::vector<SamplerLayoutElement>& samplers,
			const size_t maxBindResources = 1 << 10);

		~VulkanResourceLayout();

		void reset() override;

		void bindTexture(
			const size_t index,
			const std::shared_ptr<GpuTexture>& resource) override;

		void bindBuffer(
			const size_t index,
			const std::shared_ptr<GpuBuffer>& resource) override;

		void unbindResource(
			const std::shared_ptr<GpuResource>& resource) override;
		
		auto layout() const noexcept -> vk::PipelineLayout { return mPipelineLayout; }

		auto pool() const noexcept -> vk::DescriptorPool { return mDescriptorPool; }
	private:
		using BindDescriptor = std::pair<vk::DescriptorSet, size_t>;
		
		vk::PipelineLayout mPipelineLayout;
		vk::DescriptorPool mDescriptorPool;

		std::map<std::shared_ptr<GpuResource>, BindDescriptor> mDescriptors;
		
		std::vector<vk::DescriptorSetLayout> mDescriptorSetLayouts;
		std::vector<vk::WriteDescriptorSet> mWriteDescriptorSets;
		std::vector<size_t> mDescriptorBindings;
	};

}

#endif