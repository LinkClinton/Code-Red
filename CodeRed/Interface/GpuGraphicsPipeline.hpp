#pragma once

#include "../Shared/Noncopyable.hpp"

#include "GpuPipelineState/GpuRasterizationState.hpp"
#include "GpuPipelineState/GpuInputAssemblyState.hpp"
#include "GpuPipelineState/GpuVertexShaderState.hpp"
#include "GpuPipelineState/GpuDepthStencilState.hpp"
#include "GpuPipelineState/GpuPixelShaderState.hpp"
#include "GpuPipelineState/GpuBlendState.hpp"

#include "GpuResourceLayout.hpp"

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
			mBlendState(blend_state) {}

		~GpuGraphicsPipeline() = default;
	protected:
		std::shared_ptr<GpuRasterizationState> mRasterizationState = nullptr;
		std::shared_ptr<GpuInputAssemblyState> mInputAssemblyState = nullptr;
		std::shared_ptr<GpuVertexShaderState> mVertexShaderState = nullptr;
		std::shared_ptr<GpuDepthStencilState> mDepthStencilState = nullptr;
		std::shared_ptr<GpuPixelShaderState> mPixelShaderState = nullptr;
		std::shared_ptr<GpuResourceLayout> mResourceLayout = nullptr;
		std::shared_ptr<GpuBlendState> mBlendState = nullptr;
	};
	
}