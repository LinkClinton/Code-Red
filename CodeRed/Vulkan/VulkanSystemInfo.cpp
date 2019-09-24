#include "VulkanDisplayAdapter.hpp"
#include "VulkanSystemInfo.hpp"

#ifdef __ENABLE__VULKAN__

auto CodeRed::VulkanSystemInfo::selectDisplayAdapter() const
	-> std::vector<std::shared_ptr<GpuDisplayAdapter>>
{
	vk::ApplicationInfo appInfo = {};
	vk::InstanceCreateInfo instanceInfo = {};

	appInfo
		.setPNext(nullptr)
		.setPApplicationName("CodeRed")
		.setApplicationVersion(1)
		.setPEngineName("CodeRed")
		.setEngineVersion(1)
		.setEngineVersion(VK_API_VERSION_1_1);

	instanceInfo
		.setPNext(nullptr)
		.setFlags(vk::InstanceCreateFlags(0))
		.setPApplicationInfo(&appInfo)
		.setPpEnabledLayerNames(nullptr)
		.setPpEnabledExtensionNames(nullptr)
		.setEnabledLayerCount(0)
		.setEnabledExtensionCount(0);

	std::vector<std::shared_ptr<GpuDisplayAdapter>> displayAdapters;

	auto instance = vk::createInstance(instanceInfo);
	auto physicalDevices = instance.enumeratePhysicalDevices();
	
	for (auto& physicalDevice : physicalDevices) {
		auto properties = physicalDevice.getProperties();

		displayAdapters.push_back(
			std::make_shared<VulkanDisplayAdapter>(
				properties.deviceName, properties.deviceID, properties.vendorID));
	}

	instance.destroy();
	
	return displayAdapters;
}

#endif