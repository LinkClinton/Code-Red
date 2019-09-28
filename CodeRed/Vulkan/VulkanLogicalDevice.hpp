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
			const std::shared_ptr<GpuTexture>& render_target,
			const std::shared_ptr<GpuTexture>& depth_stencil)
			->std::shared_ptr<GpuFrameBuffer> override;

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
			const std::vector<ResourceLayoutElement>& elements,
			const std::vector<SamplerLayoutElement>& samplers,
			const size_t maxBindResources = 1 << 10)
			->std::shared_ptr<GpuResourceLayout> override;

		auto createRenderPass(
			const std::optional<Attachment>& color, 
			const std::optional<Attachment>& depth = std::nullopt)
			-> std::shared_ptr<GpuRenderPass> override;
		
		auto createSampler(const SamplerInfo& info)
			->std::shared_ptr<GpuSampler> override;

		auto createSwapChain(
			const std::shared_ptr<GpuCommandQueue>& queue,
			const WindowInfo& info,
			const PixelFormat& format,
			const size_t buffer_count)
			->std::shared_ptr<GpuSwapChain> override;

		auto createBuffer(const ResourceInfo& info)
			->std::shared_ptr<GpuBuffer> override;

		auto createTexture(const ResourceInfo& info)
			->std::shared_ptr<GpuTexture> override;

		auto createPipelineFactory()
			->std::shared_ptr<GpuPipelineFactory> override;
		
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

		vk::DebugUtilsMessengerEXT mDebugUtilsMessenger;
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