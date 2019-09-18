#pragma once

#include <Shared/Noncopyable.hpp>

#include <Interface/GpuPipelineState/GpuRasterizationState.hpp>
#include <Interface/GpuPipelineState/GpuInputAssemblyState.hpp>
#include <Interface/GpuPipelineState/GpuVertexShaderState.hpp>
#include <Interface/GpuPipelineState/GpuDepthStencilState.hpp>
#include <Interface/GpuPipelineState/GpuPixelShaderState.hpp>
#include <Interface/GpuPipelineState/GpuBlendState.hpp>

#include <Interface/GpuResourceLayout.hpp>

#include <memory>

namespace CodeRed {

	class GpuLogicalDevice;
	
	class GpuGraphicsPipeline : public Noncopyable {
	protected:
		explicit GpuGraphicsPipeline(
			const std::shared_ptr<GpuLogicalDevice>& device,
			const std::shared_ptr<GpuResourceLayout>& resource_layout,
			const std::shared_ptr<GpuInputAssemblyState>& input_assembly_state,
			const std::shared_ptr<GpuVertexShaderState>& vertex_shader_state,
			const std::shared_ptr<GpuPixelShaderState>& pixel_shader_state,
			const std::shared_ptr<GpuDepthStencilState>& depth_stencil_state,
			const std::shared_ptr<GpuBlendState>& blend_state,
			const std::shared_ptr<GpuRasterizationState>& rasterization_state) :
			mRasterizationState(rasterization_state),
			mInputAssemblyState(input_assembly_state),
			mVertexShaderState(vertex_shader_state),
			mDepthStencilState(depth_stencil_state),
			mPixelShaderState(pixel_shader_state),
			mResourceLayout(resource_layout),
			mBlendState(blend_state),
			mDevice(device) {}

		~GpuGraphicsPipeline() = default;
	protected:
		std::shared_ptr<GpuRasterizationState> mRasterizationState;
		std::shared_ptr<GpuInputAssemblyState> mInputAssemblyState;
		std::shared_ptr<GpuVertexShaderState> mVertexShaderState;
		std::shared_ptr<GpuDepthStencilState> mDepthStencilState;
		std::shared_ptr<GpuPixelShaderState> mPixelShaderState;
		std::shared_ptr<GpuResourceLayout> mResourceLayout;
		std::shared_ptr<GpuBlendState> mBlendState;

		std::shared_ptr<GpuLogicalDevice> mDevice;
	};
	
}