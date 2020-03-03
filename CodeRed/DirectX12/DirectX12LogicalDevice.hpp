#pragma once

#include "../Interface/GpuLogicalDevice.hpp"
#include "DirectX12Utility.hpp"

#ifdef __ENABLE__DIRECTX12__

namespace CodeRed {

	class DirectX12LogicalDevice final : public GpuLogicalDevice {
	public:
		explicit DirectX12LogicalDevice(
			const std::shared_ptr<GpuDisplayAdapter>& adapter);

		~DirectX12LogicalDevice() = default;

		auto createFence() -> std::shared_ptr<GpuFence> override;

		auto createFrameBuffer(
			const std::vector<std::shared_ptr<GpuTextureRef>>& render_targets, 
			const std::shared_ptr<GpuTextureRef>& depth_stencil)
			-> std::shared_ptr<GpuFrameBuffer> override;
		
		auto createGraphicsCommandList(
			const std::shared_ptr<GpuCommandAllocator>& allocator)
			-> std::shared_ptr<GpuGraphicsCommandList> override;
		
		auto createCommandQueue()
			-> std::shared_ptr<GpuCommandQueue> override;

		auto createCommandAllocator()
			-> std::shared_ptr<GpuCommandAllocator> override;

		auto createGraphicsPipeline(
			const std::shared_ptr<GpuRenderPass>& render_pass,
			const std::shared_ptr<GpuResourceLayout>& resource_layout, 
			const std::shared_ptr<GpuInputAssemblyState>& input_assembly_state, 
			const std::shared_ptr<GpuShaderState>& vertex_shader_state, 
			const std::shared_ptr<GpuShaderState>& pixel_shader_state, 
			const std::shared_ptr<GpuDepthStencilState>& depth_stencil_state, 
			const std::shared_ptr<GpuBlendState>& blend_state, 
			const std::shared_ptr<GpuRasterizationState>& rasterization_state)
			-> std::shared_ptr<GpuGraphicsPipeline> override;

		auto createResourceLayout(
			const std::vector<ResourceLayoutElement>& elements = {},
			const std::vector<SamplerLayoutElement>& samplers = {},
			const std::optional<Constant32Bits>& constant32Bits = std::nullopt)
			-> std::shared_ptr<GpuResourceLayout> override;

		auto createDescriptorHeap(
			const std::shared_ptr<GpuResourceLayout>& resource_layout)
			-> std::shared_ptr<GpuDescriptorHeap> override;

		auto createRenderPass(
			const std::vector<Attachment>& colors, 
			const std::optional<Attachment>& depth)
			-> std::shared_ptr<GpuRenderPass> override;
		
		auto createSampler(const SamplerInfo& info)
			-> std::shared_ptr<GpuSampler> override;

		auto createSwapChain(
			const std::shared_ptr<GpuCommandQueue>& queue,
			const WindowInfo& info, 
			const PixelFormat& format, 
			const size_t buffer_count)
			-> std::shared_ptr<GpuSwapChain> override;

		auto createTextureBuffer(const TextureBufferInfo& info)
			-> std::shared_ptr<GpuTextureBuffer> override;

		auto createTextureBuffer(
			const std::shared_ptr<GpuTexture>& texture,  
			const size_t mipSlice = 0)
			-> std::shared_ptr<GpuTextureBuffer> override;
		
		auto createBuffer(const ResourceInfo& info)
			-> std::shared_ptr<GpuBuffer> override;

		auto createTexture(const ResourceInfo& info)
			-> std::shared_ptr<GpuTexture> override;

		auto createPipelineFactory()
			-> std::shared_ptr<GpuPipelineFactory> override;
		
		auto device() const noexcept -> WRL::ComPtr<ID3D12Device> { return mDevice; }
	private:
		WRL::ComPtr<ID3D12Device> mDevice;
	};
	
}

#endif