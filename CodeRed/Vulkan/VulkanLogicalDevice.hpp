#pragma once

#include "../Interface/GpuLogicalDevice.hpp"
#include "VulkanUtility.hpp"

#ifdef __ENABLE__VULKAN__

namespace CodeRed {

	class VulkanLogicalDevice final : public GpuLogicalDevice {
	public:
		explicit VulkanLogicalDevice(
			const std::shared_ptr<GpuDisplayAdapter>& adapter);

		~VulkanLogicalDevice();

		auto device() const noexcept -> vk::Device { return mDevice; }
		
		auto queueFamilyIndex() const noexcept -> size_t { return mQueueFamilyIndex; }
	private:
		void initializeLayers();

		void initializeExtensions();

		void initializeDynamicLoader();
		
		void initializeDebugReport();

		auto allocateQueue() -> size_t;

		void freeQueue(const size_t index);

		auto getMemoryTypeIndex(
			uint32_t type_bits, 
			const vk::MemoryPropertyFlags& flags)
			const -> uint32_t;

		friend class VulkanCommandQueue;
		friend class VulkanSwapChain;
		friend class VulkanTexture;
		friend class VulkanBuffer;
	private:
		std::vector<const char*> mInstanceExtensions;
		std::vector<const char*> mDeviceExtensions;
		std::vector<const char*> mInstanceLayers;

		vk::DebugReportCallbackEXT mDebugReportCallBack;
		vk::DispatchLoaderDynamic mDynamicLoader;
		
		vk::PhysicalDeviceMemoryProperties mMemoryProperties;
		vk::PhysicalDevice mPhysicalDevice;
		
		vk::Instance mInstance;
		vk::Device mDevice;

		size_t mQueueFamilyIndex = SIZE_MAX;
		
		std::vector<size_t> mFreeQueues;

		bool mEnableValidationLayer = false;
		
	};
	
}

#endif