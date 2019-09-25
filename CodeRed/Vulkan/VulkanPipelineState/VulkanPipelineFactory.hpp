#pragma once

#include "../../Interface/GpuPipelineState/GpuPipelineFactory.hpp"

#ifdef __ENABLE__VULKAN__

namespace CodeRed {

	class VulkanPipelineFactory final : public GpuPipelineFactory {
	public:
		explicit VulkanPipelineFactory(
			const std::shared_ptr<GpuLogicalDevice>& device);

		~VulkanPipelineFactory() = default;
	};
	
}

#endif