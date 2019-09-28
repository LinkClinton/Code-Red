#pragma once

#include "../../Shared/StencilOperatorInfo.hpp"
#include "../../Shared/LayoutElement.hpp"
#include "../../Shared/BlendProperty.hpp"
#include "../../Shared/Noncopyable.hpp"

#include "../../Shared/Enum/PrimitiveTopology.hpp"
#include "../../Shared/Enum/ShaderType.hpp"
#include "../../Shared/Enum/FrontFace.hpp"
#include "../../Shared/Enum/CullMode.hpp"
#include "../../Shared/Enum/FillMode.hpp"

#include <vector>
#include <memory>

namespace CodeRed {


	class GpuLogicalDevice;
	class GpuInputAssemblyState;
	class GpuRasterizationState;
	class GpuDepthStencilState;
	class GpuShaderState;
	class GpuBlendState;
	
	class GpuPipelineFactory : public Noncopyable {
	protected:
		explicit GpuPipelineFactory(
			const std::shared_ptr<GpuLogicalDevice> &device) :
			mDevice(device) {}

		virtual ~GpuPipelineFactory() = default;
	public:
		virtual auto createInputAssemblyState(
			const std::vector<InputLayoutElement>& elements,
			const PrimitiveTopology primitive_topology = PrimitiveTopology::TriangleList)
			-> std::shared_ptr<GpuInputAssemblyState> = 0;

		virtual auto createRasterizationState(
			const FrontFace front_face = FrontFace::Clockwise,
			const CullMode cull_mode = CullMode::Back,
			const FillMode fill_mode = FillMode::Solid,
			const bool depth_clamp = true)
			-> std::shared_ptr<GpuRasterizationState> = 0;

		virtual auto createDetphStencilState(
			const bool depth_enable = true,
			const bool depth_write_enable = true,
			const bool stencil_enable = false,
			const CompareOperator depth_operator = CompareOperator::LessEqual,
			const StencilOperatorInfo& front = StencilOperatorInfo(),
			const StencilOperatorInfo& back = StencilOperatorInfo())
			-> std::shared_ptr<GpuDepthStencilState> = 0;

		virtual auto createShaderState(
			const ShaderType type,
			const std::vector<Byte>& code,
			const std::string& name = "main")
			-> std::shared_ptr<GpuShaderState> = 0;

		virtual auto createBlendState(
			const BlendProperty& property = BlendProperty())
			-> std::shared_ptr<GpuBlendState> = 0;
	protected:
		std::shared_ptr<GpuLogicalDevice> mDevice;
	};
	
}