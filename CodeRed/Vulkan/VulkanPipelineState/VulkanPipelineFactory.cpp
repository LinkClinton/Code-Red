#include "VulkanPipelineFactory.hpp"

#include "VulkanInputAssemblyState.hpp"
#include "VulkanRasterizationState.hpp"
#include "VulkanDepthStencilState.hpp"
#include "VulkanShaderState.hpp"
#include "VulkanBlendState.hpp"

#ifdef __ENABLE__VULKAN__

using namespace CodeRed::Vulkan;

CodeRed::VulkanPipelineFactory::VulkanPipelineFactory(
	const std::shared_ptr<GpuLogicalDevice>& device) :
	GpuPipelineFactory(device)
{
	
}

auto CodeRed::VulkanPipelineFactory::createInputAssemblyState(
	const std::vector<InputLayoutElement>& elements,
	const PrimitiveTopology primitive_topology)
	-> std::shared_ptr<GpuInputAssemblyState>
{
	return std::make_shared<VulkanInputAssemblyState>(
		mDevice,
		elements,
		primitive_topology);
}

auto CodeRed::VulkanPipelineFactory::createRasterizationState(
	const FrontFace front_face,
	const CullMode cull_mode, 
	const FillMode fill_mode,
	const bool depth_clamp)
	-> std::shared_ptr<GpuRasterizationState>
{
	return std::make_shared<VulkanRasterizationState>(
		mDevice,
		front_face,
		cull_mode,
		fill_mode,
		depth_clamp);
}

auto CodeRed::VulkanPipelineFactory::createDetphStencilState(
	const bool depth_enable,
	const bool depth_write_enable, 
	const bool stencil_enable, 
	const CompareOperator depth_operator,
	const StencilOperatorInfo& front, 
	const StencilOperatorInfo& back)
	-> std::shared_ptr<GpuDepthStencilState>
{
	return std::make_shared<VulkanDepthStencilState>(
		mDevice,
		depth_enable,
		depth_write_enable,
		stencil_enable,
		depth_operator,
		front,
		back);
}

auto CodeRed::VulkanPipelineFactory::createShaderState(
	const ShaderType type, 
	const std::vector<Byte>& code,
	const std::string& name)
	-> std::shared_ptr<GpuShaderState>
{
	return std::make_shared<VulkanShaderState>(
		mDevice,
		type,
		code,
		name);
}

auto CodeRed::VulkanPipelineFactory::createBlendState(
	const BlendProperty& property)
	-> std::shared_ptr<GpuBlendState>
{
	return std::make_shared<VulkanBlendState>(
		mDevice,
		property);
}

#endif

