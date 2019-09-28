#include "VulkanDisplayAdapter.hpp"

#ifdef __ENABLE__VULKAN__

using namespace CodeRed::Vulkan;

CodeRed::VulkanDisplayAdapter::VulkanDisplayAdapter(
	const std::string& name,
	const size_t device_id,
	const size_t vendor_id) :
	GpuDisplayAdapter(name, device_id, vendor_id)
{

}

#endif