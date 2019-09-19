#include "DirectX12PipelineFactory.hpp"

#include "DirectX12InputAssemblyState.hpp"
#include "DirectX12RasterizationState.hpp"
#include "DirectX12DepthStencilState.hpp"
#include "DirectX12ShaderState.hpp"
#include "DirectX12BlendState.hpp"

#ifdef __ENABLE__DIRECTX12__

auto CodeRed::DirectX12PipelineFactory::createInputAssemblyState(
	const std::vector<InputLayoutElement>& elements,
	const PrimitiveTopology primitive_topology)
	-> std::shared_ptr<GpuInputAssemblyState>
{
	return std::static_pointer_cast<GpuInputAssemblyState>(
		std::make_shared<DirectX12InputAssemblyState>(
			elements,
			primitive_topology));
}

auto CodeRed::DirectX12PipelineFactory::createRasterizationState(
	const PixelFormat format,
	const FrontFace front_face, 
	const CullMode cull_mode, 
	const FillMode fill_mode,
	const bool depth_clamp)
	-> std::shared_ptr<GpuRasterizationState>
{
	return std::static_pointer_cast<GpuRasterizationState>(
		std::make_shared<DirectX12RasterizationState>(
			format,
			front_face,
			cull_mode,
			fill_mode,
			depth_clamp));
}

auto CodeRed::DirectX12PipelineFactory::createDetphStencilState(
	const PixelFormat format, 
	const bool depth_enable,
	const bool depth_write_enable, 
	const bool stencil_enable, 
	const CompareOperator depth_operator,
	const StencilOperatorInfo& front, 
	const StencilOperatorInfo& back)
	-> std::shared_ptr<GpuDepthStencilState>
{
	return std::static_pointer_cast<GpuDepthStencilState>(
		std::make_shared<DirectX12DepthStencilState>(
			format,
			depth_enable,
			depth_write_enable,
			stencil_enable,
			depth_operator,
			front,
			back));
}

auto CodeRed::DirectX12PipelineFactory::createShaderState(
	const std::vector<Byte>& code)
	-> std::shared_ptr<GpuShaderState>
{
	return std::static_pointer_cast<GpuShaderState>(
		std::make_shared<DirectX12ShaderState>(code));
}

auto CodeRed::DirectX12PipelineFactory::createBlendState(
	const BlendProperty& property)
	-> std::shared_ptr<GpuBlendState>
{
	return std::static_pointer_cast<GpuBlendState>(
		std::make_shared<DirectX12BlendState>(property));
}

#endif
