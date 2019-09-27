#pragma once

#include "../../Interface/GpuPipelineState/GpuPipelineFactory.hpp"

#ifdef __ENABLE__VULKAN__

namespace CodeRed {

	class VulkanPipelineFactory final : public GpuPipelineFactory {
	public:
		explicit VulkanPipelineFactory(
			const std::shared_ptr<GpuLogicalDevice>& device);

		~VulkanPipelineFactory() = default;

		auto createInputAssemblyState(
			const std::vector<InputLayoutElement>& elements,
			const PrimitiveTopology primitive_topology = PrimitiveTopology::TriangleList)
			->std::shared_ptr<GpuInputAssemblyState> override;

		auto createRasterizationState(
			const FrontFace front_face = FrontFace::Clockwise,
			const CullMode cull_mode = CullMode::Back,
			const FillMode fill_mode = FillMode::Solid,
			const bool depth_clamp = true)
			->std::shared_ptr<GpuRasterizationState> override;

		auto createDetphStencilState(
			const bool depth_enable = true,
			const bool depth_write_enable = true,
			const bool stencil_enable = false,
			const CompareOperator depth_operator = CompareOperator::LessEqual,
			const StencilOperatorInfo& front = StencilOperatorInfo(),
			const StencilOperatorInfo& back = StencilOperatorInfo())
			->std::shared_ptr<GpuDepthStencilState> override;

		auto createShaderState(
			const ShaderType type,
			const std::vector<Byte>& code,
			const std::string& name = "main")
			->std::shared_ptr<GpuShaderState> override;

		auto createBlendState(
			const BlendProperty& property = BlendProperty())
			->std::shared_ptr<GpuBlendState> override;
	};
	
}

#endif