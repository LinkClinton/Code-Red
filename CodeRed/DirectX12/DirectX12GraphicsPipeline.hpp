#pragma once

#include <Interface/GpuGraphicsPipeline.hpp>
#include <DirectX12/DirectX12Utility.hpp>

#ifdef __ENABLE__DIRECTX12__

namespace CodeRed {

	class DirectX12GraphicsPipeline final : public GpuGraphicsPipeline {
	public:
		explicit DirectX12GraphicsPipeline(
			const std::shared_ptr<GpuLogicalDevice>& device,
			const std::shared_ptr<GpuResourceLayout>& resource_layout,
			const std::shared_ptr<GpuInputAssemblyState>& input_assembly_state,
			const std::shared_ptr<GpuShaderState>& vertex_shader_state,
			const std::shared_ptr<GpuShaderState>& pixel_shader_state,
			const std::shared_ptr<GpuDepthStencilState>& depth_stencil_state,
			const std::shared_ptr<GpuBlendState>& blend_state,
			const std::shared_ptr<GpuRasterizationState>& rasterization_state);

		~DirectX12GraphicsPipeline() = default;

		auto pipeline() const noexcept -> WRL::ComPtr<ID3D12PipelineState> { return mGraphicsPipeline; }
	private:
		WRL::ComPtr<ID3D12PipelineState> mGraphicsPipeline;
	};
	
}

#endif