#include "VulkanPipelineFactory.hpp"

#ifdef __ENABLE__VULKAN__

CodeRed::VulkanPipelineFactory::VulkanPipelineFactory(
	const std::shared_ptr<GpuLogicalDevice>& device) :
	GpuPipelineFactory(device)
{
	
}

#endif

