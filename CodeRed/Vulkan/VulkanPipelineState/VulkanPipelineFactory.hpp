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
			const PrimitiveTopology primitive_topology)
			-> std::shared_ptr<GpuInputAssemblyState> override;

		auto createRasterizationState(
			const PixelFormat format, 
			const FrontFace front_face, 
			const CullMode cull_mode, 
			const FillMode fill_mode, 
			const bool depth_clamp)
			-> std::shared_ptr<GpuRasterizationState> override;

		auto createDetphStencilState(
			const PixelFormat format, 
			const bool depth_enable, 
			const bool depth_write_enable, 
			const bool stencil_enable, 
			const CompareOperator depth_operator, 
			const StencilOperatorInfo& front, 
			const StencilOperatorInfo& back)
			-> std::shared_ptr<GpuDepthStencilState> override;

		auto createShaderState(
			const ShaderType type, 
			const std::vector<Byte>& code, 
			const std::string& name = "main")
			-> std::shared_ptr<GpuShaderState> override;

		auto createBlendState(const BlendProperty& property)
			-> std::shared_ptr<GpuBlendState> override;
	};
	
}

#endif