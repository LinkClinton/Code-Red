#include "VulkanGraphicsPipeline.hpp"
#include "VulkanResourceLayout.hpp"
#include "VulkanLogicalDevice.hpp"
#include "VulkanRenderPass.hpp"

#include "VulkanPipelineState/VulkanInputAssemblyState.hpp"
#include "VulkanPipelineState/VulkanRasterizationState.hpp"
#include "VulkanPipelineState/VulkanDepthStencilState.hpp"
#include "VulkanPipelineState/VulkanShaderState.hpp"
#include "VulkanPipelineState/VulkanBlendState.hpp"

#ifdef __ENABLE__VULKAN__

using namespace CodeRed::Vulkan;

CodeRed::VulkanGraphicsPipeline::VulkanGraphicsPipeline(
	const std::shared_ptr<GpuLogicalDevice>& device,
	const std::shared_ptr<GpuRenderPass>& render_pass,
	const std::shared_ptr<GpuResourceLayout>& resource_layout,
	const std::shared_ptr<GpuInputAssemblyState>& input_assembly_state,
	const std::shared_ptr<GpuShaderState>& vertex_shader_state,
	const std::shared_ptr<GpuShaderState>& pixel_shader_state,
	const std::shared_ptr<GpuDepthStencilState>& depth_stencil_state,
	const std::shared_ptr<GpuBlendState>& blend_state,
	const std::shared_ptr<GpuRasterizationState>& rasterization_state) :
	GpuGraphicsPipeline(
		device,
		render_pass,
		resource_layout,
		input_assembly_state,
		vertex_shader_state,
		pixel_shader_state,
		depth_stencil_state,
		blend_state,
		rasterization_state
	)
{
	vk::PipelineDynamicStateCreateInfo stateInfo = {};
	vk::GraphicsPipelineCreateInfo info = {};
	vk::PipelineMultisampleStateCreateInfo sampleInfo = {};
	vk::PipelineViewportStateCreateInfo viewInfo = {};
	
	const auto vkDevice = std::static_pointer_cast<VulkanLogicalDevice>(mDevice)->device();
	
	mDynamicStates = std::vector<vk::DynamicState>(3);
	mDynamicStates[0] = vk::DynamicState::eViewport;
	mDynamicStates[1] = vk::DynamicState::eScissor;
	mDynamicStates[2] = vk::DynamicState::eBlendConstants;
	
	stateInfo
		.setPNext(nullptr)
		.setFlags(vk::PipelineDynamicStateCreateFlags(0))
		.setDynamicStateCount(static_cast<uint32_t>(mDynamicStates.size()))
		.setPDynamicStates(mDynamicStates.data());

	sampleInfo
		.setPNext(nullptr)
		.setFlags(vk::PipelineMultisampleStateCreateFlags(0))
		.setPSampleMask(nullptr)
		.setRasterizationSamples(vk::SampleCountFlagBits::e1)
		.setSampleShadingEnable(false)
		.setAlphaToCoverageEnable(false)
		.setAlphaToOneEnable(false)
		.setMinSampleShading(0.0f);

	viewInfo
		.setPNext(nullptr)
		.setFlags(vk::PipelineViewportStateCreateFlags(0))
		.setScissorCount(1)
		.setViewportCount(1)
		.setPScissors(nullptr)
		.setPViewports(nullptr);

	auto resourceLayout = std::static_pointer_cast<VulkanResourceLayout>(mResourceLayout)->layout();
	auto vertexInputState = std::static_pointer_cast<VulkanInputAssemblyState>(mInputAssemblyState)->vertexInput();
	auto inputAssemblyState = std::static_pointer_cast<VulkanInputAssemblyState>(mInputAssemblyState)->inputAssembly();
	auto rasterizationState = std::static_pointer_cast<VulkanRasterizationState>(mRasterizationState)->state();
	auto depthStencilState = std::static_pointer_cast<VulkanDepthStencilState>(mDepthStencilState)->state();
	auto colorBlendState = std::static_pointer_cast<VulkanBlendState>(mBlendState)->state();
	auto renderPass = std::static_pointer_cast<VulkanRenderPass>(mRenderPass)->renderPass();
	
	std::vector<vk::PipelineShaderStageCreateInfo> shaderStage;
	
	shaderStage.push_back(std::static_pointer_cast<VulkanShaderState>(mVertexShaderState)->stage());
	shaderStage.push_back(std::static_pointer_cast<VulkanShaderState>(mPixelShaderState)->stage());
	
	info
		.setPNext(nullptr)
		.setFlags(vk::PipelineCreateFlags(0))
		.setLayout(resourceLayout)
		.setBasePipelineHandle(nullptr)
		.setBasePipelineIndex(0)
		.setPVertexInputState(&vertexInputState)
		.setPInputAssemblyState(&inputAssemblyState)
		.setPRasterizationState(&rasterizationState)
		.setPDepthStencilState(&depthStencilState)
		.setPColorBlendState(&colorBlendState)
		.setPTessellationState(nullptr)
		.setPMultisampleState(&sampleInfo)
		.setPDynamicState(&stateInfo)
		.setPViewportState(&viewInfo)
		.setStageCount(static_cast<uint32_t>(shaderStage.size()))
		.setPStages(shaderStage.data())
		.setRenderPass(renderPass)
		.setSubpass(0);

	mGraphicsPipeline = vkDevice.createGraphicsPipeline(nullptr, info);
}

CodeRed::VulkanGraphicsPipeline::~VulkanGraphicsPipeline()
{
	const auto vkDevice = std::static_pointer_cast<VulkanLogicalDevice>(mDevice)->device();

	vkDevice.destroyPipeline(mGraphicsPipeline);
}

#endif
