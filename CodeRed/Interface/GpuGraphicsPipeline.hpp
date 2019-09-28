#pragma once

#include "../Shared/Noncopyable.hpp"

#include "GpuPipelineState/GpuRasterizationState.hpp"
#include "GpuPipelineState/GpuInputAssemblyState.hpp"
#include "GpuPipelineState/GpuDepthStencilState.hpp"
#include "GpuPipelineState/GpuShaderState.hpp"
#include "GpuPipelineState/GpuBlendState.hpp"

#include "GpuResourceLayout.hpp"
#include "GpuRenderPass.hpp"

#include <memory>

namespace CodeRed {

	class GpuLogicalDevice;
	
	class GpuGraphicsPipeline : public Noncopyable {
	protected:
		explicit GpuGraphicsPipeline(
			const std::shared_ptr<GpuLogicalDevice>& device,
			const std::shared_ptr<GpuRenderPass>& render_pass,
			const std::shared_ptr<GpuResourceLayout>& resource_layout,
			const std::shared_ptr<GpuInputAssemblyState>& input_assembly_state,
			const std::shared_ptr<GpuShaderState>& vertex_shader_state,
			const std::shared_ptr<GpuShaderState>& pixel_shader_state,
			const std::shared_ptr<GpuDepthStencilState>& depth_stencil_state,
			const std::shared_ptr<GpuBlendState>& blend_state,
			const std::shared_ptr<GpuRasterizationState>& rasterization_state);

		~GpuGraphicsPipeline() = default;
	public:
		auto layout() const noexcept -> std::shared_ptr<GpuResourceLayout> { return mResourceLayout; }

		auto inputAssembly() const noexcept -> std::shared_ptr<GpuInputAssemblyState> { return mInputAssemblyState; }

		auto vertexShader() const noexcept -> std::shared_ptr<GpuShaderState> { return mVertexShaderState; }

		auto pixelShader() const noexcept -> std::shared_ptr<GpuShaderState> { return mPixelShaderState; }

		auto depthStencil() const noexcept -> std::shared_ptr<GpuDepthStencilState> { return mDepthStencilState; }

		auto blend() const noexcept -> std::shared_ptr<GpuBlendState> { return mBlendState; }

		auto renderPass() const noexcept -> std::shared_ptr<GpuRenderPass> { return mRenderPass; }

		auto rasterization() const noexcept -> std::shared_ptr<GpuRasterizationState> { return mRasterizationState; }
	protected:
		std::shared_ptr<GpuRasterizationState> mRasterizationState;
		std::shared_ptr<GpuInputAssemblyState> mInputAssemblyState;
		std::shared_ptr<GpuShaderState> mVertexShaderState;
		std::shared_ptr<GpuDepthStencilState> mDepthStencilState;
		std::shared_ptr<GpuShaderState> mPixelShaderState;
		std::shared_ptr<GpuResourceLayout> mResourceLayout;
		std::shared_ptr<GpuBlendState> mBlendState;
		std::shared_ptr<GpuRenderPass> mRenderPass;
		
		std::shared_ptr<GpuLogicalDevice> mDevice;
	};
	
}