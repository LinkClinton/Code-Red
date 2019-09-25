#pragma once

#include "../../Interface/GpuPipelineState/GpuShaderState.hpp"
#include "../VulkanUtility.hpp"

#ifdef __ENABLE__VULKAN__

namespace CodeRed {

	class VulkanShaderState final : public GpuShaderState {
	public:
		explicit VulkanShaderState(
			const std::shared_ptr<GpuLogicalDevice>& device,
			const ShaderType type,
			const std::vector<Byte>& code,
			const std::string& name = "main");

		~VulkanShaderState();
		
		auto stage() const noexcept -> vk::PipelineShaderStageCreateInfo { return mStage; }

		auto module() const noexcept -> vk::ShaderModule { return mModule; }
	private:
		vk::PipelineShaderStageCreateInfo mStage = {};
		vk::ShaderModule mModule;
	};
	
}

#endif