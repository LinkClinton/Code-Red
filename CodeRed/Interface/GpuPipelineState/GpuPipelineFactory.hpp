#pragma once

#ifdef __CODE__RED__GLOBAL__INCLUDE__
#include <Shared/StencilOperatorInfo.hpp>
#include <Shared/LayoutElement.hpp>
#include <Shared/BlendProperty.hpp>
#include <Shared/Noncopyable.hpp>

#include <Shared/Enum/PrimitiveTopology.hpp>
#include <Shared/Enum/PixelFormat.hpp>
#include <Shared/Enum/FrontFace.hpp>
#include <Shared/Enum/CullMode.hpp>
#include <Shared/Enum/FillMode.hpp>
#else
#include "../../Shared/StencilOperatorInfo.hpp"
#include "../../Shared/LayoutElement.hpp"
#include "../../Shared/BlendProperty.hpp"
#include "../../Shared/Noncopyable.hpp"

#include "../../Shared/Enum/PrimitiveTopology.hpp"
#include "../../Shared/Enum/PixelFormat.hpp"
#include "../../Shared/Enum/FrontFace.hpp"
#include "../../Shared/Enum/CullMode.hpp"
#include "../../Shared/Enum/FillMode.hpp"
#endif

#include <vector>
#include <memory>

namespace CodeRed {

	class GpuInputAssemblyState;
	class GpuRasterizationState;
	class GpuDepthStencilState;
	class GpuShaderState;
	class GpuBlendState;
	
	class GpuPipelineFactory : public Noncopyable {
	protected:
		GpuPipelineFactory() = default;

		~GpuPipelineFactory() = default;
	public:
		virtual auto createInputAssemblyState(
			const std::vector<InputLayoutElement>& elements,
			const PrimitiveTopology primitive_topology = PrimitiveTopology::Undefined)
			-> std::shared_ptr<GpuInputAssemblyState> = 0;

		virtual auto createRasterizationState(
			const PixelFormat format,
			const FrontFace front_face = FrontFace::Clockwise,
			const CullMode cull_mode = CullMode::Back,
			const FillMode fill_mode = FillMode::Solid,
			const bool depth_clamp = true)
			-> std::shared_ptr<GpuRasterizationState> = 0;

		virtual auto createDetphStencilState(
			const PixelFormat format,
			const bool depth_enable = true,
			const bool depth_write_enable = true,
			const bool stencil_enable = false,
			const CompareOperator depth_operator = CompareOperator::LessEqual,
			const StencilOperatorInfo& front = StencilOperatorInfo(),
			const StencilOperatorInfo& back = StencilOperatorInfo())
			-> std::shared_ptr<GpuDepthStencilState> = 0;

		virtual auto createShaderState(
			const std::vector<Byte>& code)
			-> std::shared_ptr<GpuShaderState> = 0;

		virtual auto createBlendState(
			const BlendProperty& property = BlendProperty())
			-> std::shared_ptr<GpuBlendState> = 0;
	};
	
}