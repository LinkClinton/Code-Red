#pragma once

#ifdef __CODE__RED__GLOBAL__INCLUDE__
#include <Interface/GpuPipelineState/GpuPipelineFactory.hpp>
#else
#include "../../Interface/GpuPipelineState/GpuPipelineFactory.hpp"
#endif

#ifdef __ENABLE__DIRECTX12__

namespace CodeRed {

	class DirectX12PipelineFactory final : public GpuPipelineFactory {
	public:
		DirectX12PipelineFactory() = default;

		~DirectX12PipelineFactory() = default;

		auto createInputAssemblyState(
			const std::vector<InputLayoutElement>& elements, 
			const PrimitiveTopology primitive_topology = PrimitiveTopology::Undefined)
			-> std::shared_ptr<GpuInputAssemblyState> override;

		auto createRasterizationState(
			const PixelFormat format,
			const FrontFace front_face = FrontFace::Clockwise,
			const CullMode cull_mode = CullMode::Back,
			const FillMode fill_mode = FillMode::Solid,
			const bool depth_clamp = true)
			-> std::shared_ptr<GpuRasterizationState> override;

		auto createDetphStencilState(
			const PixelFormat format,
			const bool depth_enable = true,
			const bool depth_write_enable = true,
			const bool stencil_enable = false,
			const CompareOperator depth_operator = CompareOperator::LessEqual,
			const StencilOperatorInfo& front = StencilOperatorInfo(),
			const StencilOperatorInfo& back = StencilOperatorInfo())
			->std::shared_ptr<GpuDepthStencilState> override;

		auto createShaderState(
			const std::vector<Byte>& code)
			->std::shared_ptr<GpuShaderState> override;

		auto createBlendState(
			const BlendProperty& property = BlendProperty())
			->std::shared_ptr<GpuBlendState> override;
	};
	
}

#endif