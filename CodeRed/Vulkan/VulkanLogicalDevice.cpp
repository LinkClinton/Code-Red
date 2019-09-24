#include "../Shared/DebugReport.hpp"
#include "../Shared/Exception/InvalidException.hpp"
#include "../Shared/Exception/FailedException.hpp"

#include "VulkanDisplayAdapter.hpp"
#include "VulkanLogicalDevice.hpp"


#ifdef __ENABLE__VULKAN__

#pragma comment(lib, "vulkan-1.lib")

static VKAPI_ATTR VkBool32 VKAPI_CALL debugReportCallBack(
	VkDebugReportFlagsEXT flags,
	VkDebugReportObjectTypeEXT objectType,
	uint64_t object,
	size_t location,
	int32_t messageCode,
	const char* layerPrefix,
	const char* message,
	void* userData)
{
	CodeRed::DebugReport::message(std::string("vulkan validation layer : ") + message);
	
	return VK_FALSE;
}

CodeRed::VulkanLogicalDevice::VulkanLogicalDevice(const std::shared_ptr<GpuDisplayAdapter>& adapter)
	: GpuLogicalDevice(adapter)
{
	initializeLayers();
	initializeExtensions();
	
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
		.setPpEnabledLayerNames(mInstanceLayers.data())
		.setPpEnabledExtensionNames(mInstanceExtensions.data())
		.setEnabledLayerCount(static_cast<uint32_t>(mInstanceLayers.size()))
		.setEnabledExtensionCount(static_cast<uint32_t>(mInstanceExtensions.size()));
	
	mInstance = vk::createInstance(instanceInfo);

	initializeDynamicLoader();
	initializeDebugReport();

	auto physicalDevices = mInstance.enumeratePhysicalDevices();
	auto foundPhysicalDevice = false;
	
	for (auto physicalDevice : physicalDevices) {
		auto properties = physicalDevice.getProperties();

		if (properties.deviceName == mDisplayAdapter->name() &&
			properties.deviceID == mDisplayAdapter->deviceId() &&
			properties.vendorID == mDisplayAdapter->vendorId()) {
			mPhysicalDevice = physicalDevice;

			foundPhysicalDevice = true;
			
			break;
		}
	}

	CODE_RED_THROW_IF(
		foundPhysicalDevice == false,
		InvalidException<GpuDisplayAdapter>({ "adapter" })
	);

	mMemoryProperties = mPhysicalDevice.getMemoryProperties();

	auto queueFamilyProperties = mPhysicalDevice.getQueueFamilyProperties();

	for (size_t index = 0; index < queueFamilyProperties.size(); index++) {
		if ((queueFamilyProperties[index].queueFlags & 
			(vk::QueueFlagBits::eGraphics | vk::QueueFlagBits::eTransfer))
#ifdef _WIN32
			&& mPhysicalDevice.getWin32PresentationSupportKHR(static_cast<uint32_t>(index))
#endif
			) {
			
			mQueueFamilyIndex = static_cast<int>(index);
			
			mFreeQueues.resize(queueFamilyProperties[index].queueCount);

			break;
		}
	}

	CODE_RED_THROW_IF(
		mQueueFamilyIndex == SIZE_MAX,
		FailedException({ "vk::Device" }, "no queue family supprted.", DebugType::Create)
	);
	
	for (size_t index = 0; index < mFreeQueues.size(); index++) 
		mFreeQueues[index] = mFreeQueues.size() - index - 1;

	std::vector<float> queuePriorities(mFreeQueues.size(), 0.0f);
	
	vk::DeviceQueueCreateInfo queueInfo = {};
	vk::DeviceCreateInfo deviceInfo = {};

	queueInfo
		.setPNext(nullptr)
		.setFlags(vk::DeviceQueueCreateFlags(0))
		.setPQueuePriorities(queuePriorities.data())
		.setQueueCount(static_cast<uint32_t>(mFreeQueues.size()))
		.setQueueFamilyIndex(static_cast<uint32_t>(mQueueFamilyIndex));

	deviceInfo
		.setPNext(nullptr)
		.setFlags(vk::DeviceCreateFlags(0))
		.setQueueCreateInfoCount(1)
		.setPQueueCreateInfos(&queueInfo)
		.setEnabledLayerCount(0)
		.setEnabledExtensionCount(static_cast<uint32_t>(mDeviceExtensions.size()))
		.setPpEnabledLayerNames(nullptr)
		.setPpEnabledExtensionNames(mDeviceExtensions.data())
		.setPEnabledFeatures(nullptr);

	mDevice = mPhysicalDevice.createDevice(deviceInfo);
}

CodeRed::VulkanLogicalDevice::~VulkanLogicalDevice()
{
	mDevice.destroy();
	
	if (mEnableValidationLayer == true)
		mInstance.destroyDebugReportCallbackEXT(mDebugReportCallBack, nullptr, mDynamicLoader);

	mInstance.destroy();
}

void CodeRed::VulkanLogicalDevice::initializeExtensions()
{
	mInstanceExtensions.push_back(VK_KHR_SURFACE_EXTENSION_NAME);
#ifdef VK_USE_PLATFORM_WIN32_KHR
	mInstanceExtensions.push_back(VK_KHR_WIN32_SURFACE_EXTENSION_NAME);
#endif
	mDeviceExtensions.push_back(VK_KHR_SWAPCHAIN_EXTENSION_NAME);
}

void CodeRed::VulkanLogicalDevice::initializeDynamicLoader()
{
	if (mEnableValidationLayer == true) {
		mDynamicLoader.vkCreateDebugReportCallbackEXT = reinterpret_cast<PFN_vkCreateDebugReportCallbackEXT>(
			vkGetInstanceProcAddr(mInstance, "vkCreateDebugReportCallbackEXT"));
		mDynamicLoader.vkDestroyDebugReportCallbackEXT = reinterpret_cast<PFN_vkDestroyDebugReportCallbackEXT>(
			vkGetInstanceProcAddr(mInstance, "vkDestroyDebugReportCallbackEXT"));
	}
}

void CodeRed::VulkanLogicalDevice::initializeLayers()
{
	std::vector<const char*> enableLayers;
#ifdef _DEBUG
	enableLayers.push_back("VK_LAYER_LUNARG_standard_validation");
#endif

	auto availableLayers = vk::enumerateInstanceLayerProperties();

	for (auto enableLayer : enableLayers) {
		auto available = false;

		//check the layer we can enable
		for (auto availableLayer : availableLayers) {
			if (std::strcmp(enableLayer, availableLayer.layerName) == 0) {
				
				if (std::strcmp(enableLayer, "VK_LAYER_LUNARG_standard_validation") == 0) {
					mEnableValidationLayer = true;

					mInstanceExtensions.push_back(VK_EXT_DEBUG_REPORT_EXTENSION_NAME);
				}

				mInstanceLayers.push_back(enableLayer);
				available = true;
				break;
			}
		}

		if (available == false)
			DebugReport::warning("enable [0] layer failed.", { enableLayer });
	}
}

void CodeRed::VulkanLogicalDevice::initializeDebugReport()
{
	if (mEnableValidationLayer == false) return;

	vk::DebugReportCallbackCreateInfoEXT info = {};

	info.setFlags(
		vk::DebugReportFlagBitsEXT::ePerformanceWarning |
		vk::DebugReportFlagBitsEXT::eInformation |
		vk::DebugReportFlagBitsEXT::eWarning |
		vk::DebugReportFlagBitsEXT::eDebug |
		vk::DebugReportFlagBitsEXT::eError)
		.setPfnCallback(debugReportCallBack);

	mDebugReportCallBack = mInstance.createDebugReportCallbackEXT(info, nullptr, mDynamicLoader);
}

auto CodeRed::VulkanLogicalDevice::allocateQueue() -> size_t
{
	CODE_RED_THROW_IF(
		mFreeQueues.empty(),
		FailedException({ "vk::Queue", "vk::Device" }, "too many queues were allocated.", DebugType::Get)
	);

	const auto index = mFreeQueues.back();
	
	mFreeQueues.pop_back();
	
	return index;
}

void CodeRed::VulkanLogicalDevice::freeQueue(const size_t index)
{
	const auto it = std::find(mFreeQueues.begin(), mFreeQueues.end(), index);
	
	CODE_RED_THROW_IF(
		it != mFreeQueues.end(),
		InvalidException<size_t>({ "index" }, "the queue has freed.")
	);

	mFreeQueues.push_back(index);
}

#endif
