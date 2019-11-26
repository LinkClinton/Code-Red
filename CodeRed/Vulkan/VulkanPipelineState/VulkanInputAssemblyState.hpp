#pragma once

#include "../../Interface/GpuPipelineState/GpuInputAssemblyState.hpp"
#include "../VulkanUtility.hpp"

#ifdef __ENABLE__VULKAN__

namespace CodeRed {

	class VulkanInputAssemblyState final : public GpuInputAssemblyState {
	public:
		explicit VulkanInputAssemblyState(
			const std::shared_ptr<GpuLogicalDevice>& device,
			const std::vector<InputLayoutElement>& elements,
			const PrimitiveTopology primitive_topology = PrimitiveTopology::TriangleList);
		
		~VulkanInputAssemblyState() = default;

		auto inputAssembly() const noexcept -> vk::PipelineInputAssemblyStateCreateInfo { return mInputAssembly; }

		auto vertexInput() const noexcept -> vk::PipelineVertexInputStateCreateInfo { return mVertexInput; }
	private:
		std::vector<vk::VertexInputAttributeDescription> mVertexAttributes = {};
		std::vector<vk::VertexInputBindingDescription> mVertexBindings = {};
		
		vk::PipelineInputAssemblyStateCreateInfo mInputAssembly = {};
		vk::PipelineVertexInputStateCreateInfo mVertexInput = {};
	};
	
}

#endif