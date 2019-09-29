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
			const std::vector<SamplerLayoutElement>& samplers);

		~VulkanResourceLayout();

		auto layout() const noexcept -> vk::PipelineLayout { return mPipelineLayout; }
	private:
		friend class VulkanDescriptorHeap;
		
		vk::PipelineLayout mPipelineLayout;

		std::vector<vk::DescriptorSetLayout> mDescriptorSetLayouts;
	};

}

#endif