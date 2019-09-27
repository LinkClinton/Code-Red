#pragma once

#include "../Interface/GpuGraphicsPipeline.hpp"
#include "VulkanUtility.hpp"

#ifdef __ENABLE__VULKAN__

namespace CodeRed {

	class VulkanGraphicsPipeline final : public GpuGraphicsPipeline {
	public:
		explicit VulkanGraphicsPipeline(
			const std::shared_ptr<GpuLogicalDevice>& device,
			const std::shared_ptr<GpuRenderPass>& render_pass,
			const std::shared_ptr<GpuResourceLayout>& resource_layout,
			const std::shared_ptr<GpuInputAssemblyState>& input_assembly_state,
			const std::shared_ptr<GpuShaderState>& vertex_shader_state,
			const std::shared_ptr<GpuShaderState>& pixel_shader_state,
			const std::shared_ptr<GpuDepthStencilState>& depth_stencil_state,
			const std::shared_ptr<GpuBlendState>& blend_state,
			const std::shared_ptr<GpuRasterizationState>& rasterization_state);

		~VulkanGraphicsPipeline();

		auto pipeline() const noexcept -> vk::Pipeline { return mGraphicsPipeline; }
	private:
		std::vector<vk::DynamicState> mDynamicStates = {};
		
		vk::Pipeline mGraphicsPipeline;
	};
	
}

#endif