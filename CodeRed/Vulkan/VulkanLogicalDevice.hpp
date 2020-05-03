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

		auto createFence()->std::shared_ptr<GpuFence> override;

		auto createFrameBuffer(
			const std::vector<std::shared_ptr<GpuTextureRef>>& render_targets, 
			const std::shared_ptr<GpuTextureRef>& depth_stencil)
			-> std::shared_ptr<GpuFrameBuffer> override;
		
		auto createGraphicsCommandList(
			const std::shared_ptr<GpuCommandAllocator>& allocator)
			->std::shared_ptr<GpuGraphicsCommandList> override;

		auto createCommandQueue()
			->std::shared_ptr<GpuCommandQueue> override;

		auto createCommandAllocator()
			->std::shared_ptr<GpuCommandAllocator> override;

		auto createGraphicsPipeline(
			const std::shared_ptr<GpuRenderPass> &render_pass,
			const std::shared_ptr<GpuResourceLayout>& resource_layout,
			const std::shared_ptr<GpuInputAssemblyState>& input_assembly_state,
			const std::shared_ptr<GpuShaderState>& vertex_shader_state,
			const std::shared_ptr<GpuShaderState>& pixel_shader_state,
			const std::shared_ptr<GpuDepthStencilState>& depth_stencil_state,
			const std::shared_ptr<GpuBlendState>& blend_state,
			const std::shared_ptr<GpuRasterizationState>& rasterization_state)
			->std::shared_ptr<GpuGraphicsPipeline> override;

		auto createResourceLayout(
			const std::vector<ResourceLayoutElement>& elements = {},
			const std::vector<SamplerLayoutElement>& samplers = {},
			const std::optional<Constant32Bits>& constant32Bits = std::nullopt)
			->std::shared_ptr<GpuResourceLayout> override;

		auto createDescriptorHeap(
			const std::shared_ptr<GpuResourceLayout>& resource_layout)
			-> std::shared_ptr<GpuDescriptorHeap> override;

		auto createRenderPass(
			const std::vector<Attachment>& colors, 
			const std::optional<Attachment>& depth)
			-> std::shared_ptr<GpuRenderPass> override;
		
		auto createSampler(const SamplerInfo& info)
			->std::shared_ptr<GpuSampler> override;

		auto createSwapChain(
			const std::shared_ptr<GpuCommandQueue>& queue,
			const WindowInfo& info,
			const PixelFormat& format,
			const size_t buffer_count)
			->std::shared_ptr<GpuSwapChain> override;

		auto createTextureBuffer(const TextureBufferInfo& info)
			-> std::shared_ptr<GpuTextureBuffer> override;

		auto createTextureBuffer(
			const std::shared_ptr<GpuTexture>& texture, 
			const size_t mipSlice)
			-> std::shared_ptr<GpuTextureBuffer> override;
		
		auto createBuffer(const ResourceInfo& info)
			->std::shared_ptr<GpuBuffer> override;

		auto createTexture(const ResourceInfo& info)
			->std::shared_ptr<GpuTexture> override;

		auto createPipelineFactory()
			->std::shared_ptr<GpuPipelineFactory> override;
		
		auto device() const noexcept -> vk::Device { return mDevice; }

		auto queueFamilyIndex() const noexcept -> size_t { return mQueueFamilyIndex; }

		static auto instance() -> vk::Instance;
	private:
		static void initializeLayers();

		static void initializeExtensions();

		static void initializeInstance();
		
		void initializeDynamicLoader();
		
		void initializeDebugReport();

		void initializeFeatures();

		auto allocateQueue() -> size_t;

		void freeQueue(const size_t index);

		auto getMemoryTypeIndex(
			uint32_t type_bits, 
			const vk::MemoryPropertyFlags& flags)
			const -> uint32_t;

		friend class VulkanTextureBuffer;
		friend class VulkanCommandQueue;
		friend class VulkanSwapChain;
		friend class VulkanTexture;
		friend class VulkanBuffer;
	private:
		static inline vk::Instance mInstance = nullptr;
		
		static inline std::vector<const char*> mInstanceExtensions;
		static inline std::vector<const char*> mDeviceExtensions;
		static inline std::vector<const char*> mInstanceLayers;

		static inline bool mEnableValidationLayer = false;
		
		vk::DebugUtilsMessengerEXT mDebugUtilsMessenger;
		vk::DispatchLoaderDynamic mDynamicLoader;
		
		vk::PhysicalDeviceMemoryProperties mMemoryProperties;
		vk::PhysicalDeviceFeatures mPhysicalFeatures;
		vk::PhysicalDevice mPhysicalDevice;
		
		vk::Device mDevice;

		size_t mQueueFamilyIndex = SIZE_MAX;
		
		std::vector<size_t> mFreeQueues;

	};
	
}

#endif