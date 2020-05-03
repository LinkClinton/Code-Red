#include "VulkanDisplayAdapter.hpp"
#include "VulkanLogicalDevice.hpp"
#include "VulkanSystemInfo.hpp"

#include "../Shared/DebugReport.hpp"

#ifdef __ENABLE__VULKAN__

using namespace CodeRed::Vulkan;

auto CodeRed::VulkanSystemInfo::selectDisplayAdapter() const
	-> std::vector<std::shared_ptr<GpuDisplayAdapter>>
{
	std::vector<std::shared_ptr<GpuDisplayAdapter>> displayAdapters;

	auto instance = VulkanLogicalDevice::instance();
	auto physicalDevices = instance.enumeratePhysicalDevices();

	CODE_RED_DEBUG_LOG("");
	CODE_RED_DEBUG_LOG("enum the adapters.");
	CODE_RED_DEBUG_LOG("");

	size_t index = 0;
	
	for (auto& physicalDevice : physicalDevices) {
		auto properties = physicalDevice.getProperties();
		
		CODE_RED_DEBUG_LOG(DebugReport::make("adapter [0] --------", { std::to_string(index++) }));
		CODE_RED_DEBUG_LOG("device name  : " + std::string(properties.deviceName));
		CODE_RED_DEBUG_LOG("device id    : " + std::to_string(properties.deviceID));
		CODE_RED_DEBUG_LOG("vendor id    : " + std::to_string(properties.vendorID));
		CODE_RED_DEBUG_LOG("");
		
		displayAdapters.push_back(
			std::make_shared<VulkanDisplayAdapter>(
				physicalDevice, properties.deviceName, properties.deviceID, properties.vendorID));
	}

	return displayAdapters;
}

#endif