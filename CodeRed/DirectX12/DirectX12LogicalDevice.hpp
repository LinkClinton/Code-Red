#pragma once

#ifdef __CODE__RED__GLOBAL__INCLUDE__
#include <Interface/GpuLogicalDevice.hpp>
#include <DirectX12/DirectX12Utility.hpp>
#else
#include "../Interface/GpuLogicalDevice.hpp"
#include "DirectX12Utility.hpp"
#endif

#ifdef __ENABLE__DIRECTX12__

namespace CodeRed {

	class DirectX12LogicalDevice final : public GpuLogicalDevice {
	public:
		explicit DirectX12LogicalDevice(
			const std::shared_ptr<GpuDisplayAdapter>& adapter);

		~DirectX12LogicalDevice() = default;

		auto createFence() -> std::shared_ptr<GpuFence> override;

		auto createFrameBuffer(
			const std::shared_ptr<GpuTexture>& render_target, 
			const std::shared_ptr<GpuTexture>& depth_stencil)
			-> std::shared_ptr<GpuFrameBuffer> override;

		auto createGraphicsCommandList(
			const std::shared_ptr<GpuCommandAllocator>& allocator)
			-> std::shared_ptr<GpuGraphicsCommandList> override;
		
		auto createCommandQueue()
			-> std::shared_ptr<GpuCommandQueue> override;

		auto createCommandAllocator()
			-> std::shared_ptr<GpuCommandAllocator> override;

		auto createGraphicsPipeline(
			const std::shared_ptr<GpuResourceLayout>& resource_layout, 
			const std::shared_ptr<GpuInputAssemblyState>& input_assembly_state, 
			const std::shared_ptr<GpuShaderState>& vertex_shader_state, 
			const std::shared_ptr<GpuShaderState>& pixel_shader_state, 
			const std::shared_ptr<GpuDepthStencilState>& depth_stencil_state, 
			const std::shared_ptr<GpuBlendState>& blend_state, 
			const std::shared_ptr<GpuRasterizationState>& rasterization_state)
			-> std::shared_ptr<GpuGraphicsPipeline> override;

		auto createResourceLayout(
			const std::vector<ResourceLayoutElement>& elements, 
			const std::vector<SamplerLayoutElement>& samplers,
			const size_t maxBindResources = 1 << 10)
			-> std::shared_ptr<GpuResourceLayout> override;

		auto createSampler(const SamplerInfo& info)
			-> std::shared_ptr<GpuSampler> override;

		auto createSwapChain(
			const std::shared_ptr<GpuCommandQueue>& queue,
			const WindowInfo& info, 
			const PixelFormat& format, 
			const size_t buffer_count)
			-> std::shared_ptr<GpuSwapChain> override;

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