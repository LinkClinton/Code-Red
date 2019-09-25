#include "../VulkanLogicalDevice.hpp"
#include "VulkanShaderState.hpp"

#include <spirv_cross/spirv_cross.hpp>

#ifdef __ENABLE__VULKAN__

CodeRed::VulkanShaderState::VulkanShaderState(
	const std::shared_ptr<GpuLogicalDevice>& device,
	const ShaderType type,
	const std::vector<Byte>& code,
	const std::string& name) :
	GpuShaderState(device, type, code, name)
{
	const auto vkDevice = std::static_pointer_cast<VulkanLogicalDevice>(mDevice)->device();

	vk::ShaderModuleCreateInfo info = {};

	info
		.setPNext(nullptr)
		.setFlags(vk::ShaderModuleCreateFlags(0))
		.setCodeSize(mCode.size() / (sizeof uint32_t / sizeof Byte))
		.setPCode(reinterpret_cast<uint32_t*>(mCode.data()));

	mModule = vkDevice.createShaderModule(info);

	mStage
		.setPNext(nullptr)
		.setFlags(vk::PipelineShaderStageCreateFlags(0))
		.setModule(mModule)
		.setPName(mName.c_str())
		.setPSpecializationInfo(nullptr)
		.setStage(enumConvert(mType));
}

CodeRed::VulkanShaderState::~VulkanShaderState()
{
	const auto vkDevice = std::static_pointer_cast<VulkanLogicalDevice>(mDevice)->device();

	vkDevice.destroyShaderModule(mModule);
}

#endif
