#include "../Shared/Exception/InvalidException.hpp"
#include "../Shared/Exception/FailedException.hpp"
#include "../Shared/DebugReport.hpp"

#include "VulkanPipelineState/VulkanPipelineFactory.hpp"

#include "VulkanResource/VulkanTextureBuffer.hpp"
#include "VulkanResource/VulkanTexture.hpp"
#include "VulkanResource/VulkanSampler.hpp"
#include "VulkanResource/VulkanBuffer.hpp"

#include "VulkanGraphicsCommandList.hpp"
#include "VulkanGraphicsPipeline.hpp"
#include "VulkanCommandAllocator.hpp"
#include "VulkanDisplayAdapter.hpp"
#include "VulkanResourceLayout.hpp"
#include "VulkanDescriptorHeap.hpp"
#include "VulkanLogicalDevice.hpp"
#include "VulkanCommandQueue.hpp"
#include "VulkanFrameBuffer.hpp"
#include "VulkanRenderPass.hpp"
#include "VulkanSwapChain.hpp"
#include "VulkanFence.hpp"

#ifdef __ENABLE__VULKAN__

using namespace CodeRed::Vulkan;

#pragma comment(lib, "vulkan-1.lib")

static VKAPI_ATTR VkBool32 VKAPI_CALL debugReportCallBack(
	const VkDebugUtilsMessageSeverityFlagBitsEXT message_severity,
	const VkDebugUtilsMessageTypeFlagsEXT message_type,
	const VkDebugUtilsMessengerCallbackDataEXT* callback_data,
	void* user_data)
{
	const static std::vector<const char*> ignoreMessages = {
		"is being used in draw but has never been updated via vkUpdateDescriptorSets() or a similar call."
	};

	for (const auto& ignoreMessage : ignoreMessages) 
		if (std::string(callback_data->pMessage).find(ignoreMessage)) return VK_FALSE;
	
	CodeRed::DebugReport::message(std::string("vulkan validation layer : ") + callback_data->pMessage);
	CodeRed::DebugReport::message("");
	
	return VK_FALSE;
}

CodeRed::VulkanLogicalDevice::VulkanLogicalDevice(const std::shared_ptr<GpuDisplayAdapter>& adapter)
	: GpuLogicalDevice(adapter, APIVersion::Vulkan)
{
	initializeLayers();
	initializeExtensions();
	
	vk::ApplicationInfo appInfo = {};
	vk::InstanceCreateInfo instanceInfo = {};

	appInfo
		.setPNext(nullptr)
		.setPApplicationName("CodeRed")
		.setApplicationVersion(VK_MAKE_VERSION(1, 0, 0))
		.setPEngineName("CodeRed")
		.setEngineVersion(VK_MAKE_VERSION(1, 0, 0))
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

	for (const auto extension : mInstanceExtensions) {
		CODE_RED_DEBUG_LOG("enabled vulkan instance extension : " + std::string(extension));
	}
		
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

	CODE_RED_DEBUG_LOG(DebugReport::make("create device with [0]", { mDisplayAdapter->name() }));
	
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

			CODE_RED_DEBUG_LOG(
				DebugReport::make(
					"create queues with queue family [0], "
					"max number of queues is [1].",
					{
						std::to_string(mQueueFamilyIndex),
						std::to_string(mFreeQueues.size())
					}
				)
			);
			
			break;
		}
	}
	
	CODE_RED_THROW_IF(
		mQueueFamilyIndex == SIZE_MAX,
		FailedException(DebugType::Create, 
			{ "vk::Device" },
			{ "no queue family supprted." })
	);
	
	for (size_t index = 0; index < mFreeQueues.size(); index++) 
		mFreeQueues[index] = mFreeQueues.size() - index - 1;

	std::vector<float> queuePriorities(mFreeQueues.size(), 0.0f);
	
	vk::DeviceQueueCreateInfo queueInfo = {};
	vk::DeviceCreateInfo deviceInfo = {};

	initializeFeatures();

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
		.setPEnabledFeatures(&mPhysicalFeatures);
	
	mDevice = mPhysicalDevice.createDevice(deviceInfo);

	for (const auto extension : mDeviceExtensions) {
		CODE_RED_DEBUG_LOG("enabled vulkan device extension : " + std::string(extension));
	}
}

CodeRed::VulkanLogicalDevice::~VulkanLogicalDevice()
{
	mDevice.destroy();
	
	if (mEnableValidationLayer == true)
		mInstance.destroyDebugUtilsMessengerEXT(mDebugUtilsMessenger, nullptr, mDynamicLoader);

	mInstance.destroy();
}

auto CodeRed::VulkanLogicalDevice::createFence()
	-> std::shared_ptr<GpuFence>
{
	return std::make_shared<VulkanFence>(shared_from_this());
}

auto CodeRed::VulkanLogicalDevice::createFrameBuffer(
	const std::vector<std::shared_ptr<GpuTextureRef>>& render_targets,
	const std::shared_ptr<GpuTextureRef>& depth_stencil)
	-> std::shared_ptr<GpuFrameBuffer>
{
	return std::make_shared<VulkanFrameBuffer>(
		shared_from_this(),
		render_targets,
		depth_stencil);
}

auto CodeRed::VulkanLogicalDevice::createGraphicsCommandList(
	const std::shared_ptr<GpuCommandAllocator>& allocator)
	-> std::shared_ptr<GpuGraphicsCommandList>
{
	return std::make_shared<VulkanGraphicsCommandList>(
		shared_from_this(),
		allocator);
}

auto CodeRed::VulkanLogicalDevice::createCommandQueue()
	-> std::shared_ptr<GpuCommandQueue>
{
	return std::make_shared<VulkanCommandQueue>(
		shared_from_this());
}

auto CodeRed::VulkanLogicalDevice::createCommandAllocator()
	-> std::shared_ptr<GpuCommandAllocator>
{
	return std::make_shared<VulkanCommandAllocator>(
		shared_from_this());
}

auto CodeRed::VulkanLogicalDevice::createGraphicsPipeline(
	const std::shared_ptr<GpuRenderPass>& render_pass,
	const std::shared_ptr<GpuResourceLayout>& resource_layout,
	const std::shared_ptr<GpuInputAssemblyState>& input_assembly_state,
	const std::shared_ptr<GpuShaderState>& vertex_shader_state,
	const std::shared_ptr<GpuShaderState>& pixel_shader_state,
	const std::shared_ptr<GpuDepthStencilState>& depth_stencil_state, 
	const std::shared_ptr<GpuBlendState>& blend_state,
	const std::shared_ptr<GpuRasterizationState>& rasterization_state)
	-> std::shared_ptr<GpuGraphicsPipeline>
{
	return std::make_shared<VulkanGraphicsPipeline>(
		shared_from_this(),
		render_pass,
		resource_layout,
		input_assembly_state,
		vertex_shader_state,
		pixel_shader_state,
		depth_stencil_state,
		blend_state,
		rasterization_state);
}

auto CodeRed::VulkanLogicalDevice::createResourceLayout(
	const std::vector<ResourceLayoutElement>& elements,
	const std::vector<SamplerLayoutElement>& samplers,
	const std::optional<Constant32Bits>& constant32Bits)
	-> std::shared_ptr<GpuResourceLayout>
{
	return std::make_shared<VulkanResourceLayout>(
		shared_from_this(),
		elements,
		samplers,
		constant32Bits);
}

auto CodeRed::VulkanLogicalDevice::createDescriptorHeap(
	const std::shared_ptr<GpuResourceLayout>& resource_layout)
	-> std::shared_ptr<GpuDescriptorHeap>
{
	return std::make_shared<VulkanDescriptorHeap>(
		shared_from_this(),
		resource_layout);
}

auto CodeRed::VulkanLogicalDevice::createRenderPass(
	const std::vector<Attachment>& colors,
	const std::optional<Attachment>& depth)
	-> std::shared_ptr<GpuRenderPass>
{
	return std::make_shared<VulkanRenderPass>(
		shared_from_this(),
		colors,
		depth);
}

auto CodeRed::VulkanLogicalDevice::createSampler(const SamplerInfo& info)
	-> std::shared_ptr<GpuSampler>
{
	return std::make_shared<VulkanSampler>(
		shared_from_this(), info);
}

auto CodeRed::VulkanLogicalDevice::createSwapChain(
	const std::shared_ptr<GpuCommandQueue>& queue,
	const WindowInfo& info,
	const PixelFormat& format, 
	const size_t buffer_count)
	-> std::shared_ptr<GpuSwapChain>
{
	return std::make_shared<VulkanSwapChain>(
		shared_from_this(),
		queue,
		info,
		format,
		buffer_count);
}

auto CodeRed::VulkanLogicalDevice::createTextureBuffer(
	const TextureBufferInfo& info)
	-> std::shared_ptr<GpuTextureBuffer>
{
	return std::make_shared<VulkanTextureBuffer>(
		shared_from_this(), info);
}

auto CodeRed::VulkanLogicalDevice::createTextureBuffer(
	const std::shared_ptr<GpuTexture>& texture,
	const size_t mipSlice)
	-> std::shared_ptr<GpuTextureBuffer>
{
	return std::make_shared<VulkanTextureBuffer>(
		shared_from_this(), texture, mipSlice);
}

auto CodeRed::VulkanLogicalDevice::createBuffer(const ResourceInfo& info)
	-> std::shared_ptr<GpuBuffer>
{
	return std::make_shared<VulkanBuffer>(
		shared_from_this(), info);
}

auto CodeRed::VulkanLogicalDevice::createTexture(const ResourceInfo& info)
	-> std::shared_ptr<GpuTexture>
{
	return std::make_shared<VulkanTexture>(
		shared_from_this(), info);
}

auto CodeRed::VulkanLogicalDevice::createPipelineFactory()
	-> std::shared_ptr<GpuPipelineFactory>
{
	return std::make_shared<VulkanPipelineFactory>(shared_from_this());
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
		mDynamicLoader.vkCreateDebugUtilsMessengerEXT = reinterpret_cast<PFN_vkCreateDebugUtilsMessengerEXT>(
			mInstance.getProcAddr("vkCreateDebugUtilsMessengerEXT"));
		mDynamicLoader.vkDestroyDebugUtilsMessengerEXT = reinterpret_cast<PFN_vkDestroyDebugUtilsMessengerEXT>(
			mInstance.getProcAddr("vkDestroyDebugUtilsMessengerEXT"));
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

					mInstanceExtensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
				}

				CODE_RED_DEBUG_LOG("enabled vulkan layer : " + std::string(enableLayer));
				
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

	vk::DebugUtilsMessengerCreateInfoEXT info = {};

	info
		.setMessageSeverity(
			vk::DebugUtilsMessageSeverityFlagBitsEXT::eWarning |
			vk::DebugUtilsMessageSeverityFlagBitsEXT::eError)
		.setMessageType(
			vk::DebugUtilsMessageTypeFlagBitsEXT::eGeneral |
			vk::DebugUtilsMessageTypeFlagBitsEXT::ePerformance |
			vk::DebugUtilsMessageTypeFlagBitsEXT::eValidation)
		.setPfnUserCallback(debugReportCallBack);
	
	mDebugUtilsMessenger = mInstance.createDebugUtilsMessengerEXT(info, nullptr, mDynamicLoader);
}

void CodeRed::VulkanLogicalDevice::initializeFeatures()
{
	mPhysicalFeatures = mPhysicalDevice.getFeatures();
}

auto CodeRed::VulkanLogicalDevice::allocateQueue() -> size_t
{
	CODE_RED_THROW_IF(
		mFreeQueues.empty(),
		FailedException(DebugType::Get,
			{ "vk::Queue", "vk::Device" }, 
			{ "too many queues were allocated." })
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
		InvalidException<size_t>(
			{ "index" },
			{ "the queue has freed." })
	);

	mFreeQueues.push_back(index);
}

auto CodeRed::VulkanLogicalDevice::getMemoryTypeIndex(
	uint32_t type_bits,
	const vk::MemoryPropertyFlags& flags) const -> uint32_t
{
	for (size_t index = 0; index < mMemoryProperties.memoryTypeCount; index++) {
		if ((type_bits & 1) == 1) {
			if ((mMemoryProperties.memoryTypes[index].propertyFlags & flags) == flags)
				return static_cast<uint32_t>(index);
		}

		type_bits >>= 1;
	}

	throw FailedException(DebugType::Get, { "memory type index", "memory properties" });
}

#endif
