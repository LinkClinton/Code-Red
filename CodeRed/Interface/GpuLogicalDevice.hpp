#pragma once

#include <Shared/Information/ResourceInfo.hpp>
#include <Shared/Information/SamplerInfo.hpp>
#include <Shared/Information/WindowInfo.hpp>
#include <Shared/LayoutElement.hpp>
#include <Shared/Noncopyable.hpp>

#include <vector>
#include <memory>

namespace CodeRed {

	class GpuRasterizationState;
	class GpuInputAssemblyState;
	class GpuDepthStencilState;
	class GpuGraphicsPipeline;
	class GpuResourceLayout;
	class GpuShaderState;
	class GpuBlendState;
	
	class GpuGraphicsCommandList;
	class GpuCommandAllocator;
	class GpuCommandQueue;
	
	class GpuDisplayAdapter;

	class GpuFrameBuffer;
	class GpuSwapChain;

	class GpuSampler;
	class GpuTexture;
	class GpuBuffer;

	class GpuFence;
	
	class GpuLogicalDevice : public Noncopyable {
	protected:
		explicit GpuLogicalDevice(const std::shared_ptr<GpuDisplayAdapter>& adapter) :
			mDisplayAdapter(adapter) {};
			
		~GpuLogicalDevice() = default;
	public:
		virtual auto createFence() -> std::shared_ptr<GpuFence> = 0;

		virtual auto createFrameBuffer(
			const std::shared_ptr<GpuTexture>& render_target,
			const std::shared_ptr<GpuTexture>& depth_stencil)
			-> std::shared_ptr<GpuFrameBuffer> = 0;

		virtual auto createGraphicsCommandList(
			const std::shared_ptr<GpuCommandAllocator> &allocator)
			-> std::shared_ptr<GpuGraphicsCommandList> = 0;

		virtual auto createCommandQueue()
			-> std::shared_ptr<GpuCommandQueue> = 0;

		virtual auto createCommandAllocator()
			->std::shared_ptr<GpuCommandAllocator> = 0;
		
		virtual auto createGraphicsPipeline(
			const std::shared_ptr<GpuResourceLayout>& resource_layout,
			const std::shared_ptr<GpuInputAssemblyState>& input_assembly_state,
			const std::shared_ptr<GpuShaderState>& vertex_shader_state,
			const std::shared_ptr<GpuShaderState>& pixel_shader_state,
			const std::shared_ptr<GpuDepthStencilState>& depth_stencil_state,
			const std::shared_ptr<GpuBlendState>& blend_state,
			const std::shared_ptr<GpuRasterizationState>& rasterization_state)
			-> std::shared_ptr<GpuGraphicsPipeline> = 0;

		virtual auto createResourceLayout(
			const std::vector<ResourceLayoutElement>& elements,
			const std::vector<SamplerLayoutElement>& samplers)
			-> std::shared_ptr<GpuResourceLayout> = 0;

		virtual auto createSampler(
			const SamplerInfo& info)
			-> std::shared_ptr<GpuSampler> = 0;

		virtual auto createSwapChain(
			const WindowInfo& info,
			const PixelFormat& format,
			const size_t buffer_count = 2)
			-> std::shared_ptr<GpuSwapChain> = 0;

		virtual auto createBuffer(
			const ResourceInfo& info)
			-> std::shared_ptr<GpuBuffer> = 0;

		virtual auto createTexture(
			const ResourceInfo& info)
			-> std::shared_ptr<GpuTexture> = 0;
	protected:
		std::shared_ptr<GpuDisplayAdapter> mDisplayAdapter;
	};
	
}