#include "../Shared/Exception/InvalidException.hpp"
#include "../Shared/Exception/FailedException.hpp"

#include "VulkanResource/VulkanTexture.hpp"
#include "VulkanResource/VulkanSampler.hpp"
#include "VulkanResource/VulkanBuffer.hpp"

#include "VulkanResourceLayout.hpp"
#include "VulkanLogicalDevice.hpp"

#undef max

#ifdef __ENABLE__VULKAN__

using namespace CodeRed::Vulkan;

CodeRed::VulkanResourceLayout::VulkanResourceLayout(
	const std::shared_ptr<GpuLogicalDevice>& device,
	const std::vector<ResourceLayoutElement>& elements,
	const std::vector<SamplerLayoutElement>& samplers,
	const std::optional<Constant32Bits>& constant32Bits) :
	GpuResourceLayout(device, elements, samplers, constant32Bits)
{
	size_t maxSpace = 0;

	//find the max space we use, then we will create layouts with maxSpace
	for (const auto element : elements) maxSpace = std::max(maxSpace, element.Space + 1);
	for (const auto sampler : samplers) maxSpace = std::max(maxSpace, sampler.Space + 1);

	std::vector<std::vector<vk::DescriptorSetLayoutBinding>> bindings(maxSpace);

	//generate the binding information of elements
	for (const auto element : mElements) {
		vk::DescriptorSetLayoutBinding binding = {};

		//for buffer and texture, we ignore the shader visibly
		binding
			.setBinding(static_cast<uint32_t>(element.Binding))
			.setDescriptorType(enumConvert(element.Type))
			.setDescriptorCount(1)
			.setStageFlags(vk::ShaderStageFlagBits::eAll)
			.setPImmutableSamplers(nullptr);

		bindings[element.Space].push_back(binding);
	}

	//generate the binding information of samplers
	for (const auto sampler : mSamplers) {
		vk::DescriptorSetLayoutBinding binding = {};

		binding
			.setBinding(static_cast<uint32_t>(sampler.Binding))
			.setDescriptorType(vk::DescriptorType::eSampler)
			.setDescriptorCount(1)
			.setStageFlags(enumConvert(sampler.Visibility))
			.setPImmutableSamplers(&std::static_pointer_cast<VulkanSampler>(sampler.Sampler)->mSampler);

		bindings[sampler.Space].push_back(binding);
	}

	const auto vkDevice = std::static_pointer_cast<VulkanLogicalDevice>(mDevice)->device();

	mDescriptorSetLayouts = std::vector<vk::DescriptorSetLayout>(maxSpace);

	//create the set layouts with bindings
	for (size_t index = 0; index < mDescriptorSetLayouts.size(); index++) {
		vk::DescriptorSetLayoutCreateInfo info = {};

		info
			.setPNext(nullptr)
			.setFlags(vk::DescriptorSetLayoutCreateFlags(0))
			.setBindingCount(static_cast<uint32_t>(bindings[index].size()))
			.setPBindings(bindings[index].data());

		mDescriptorSetLayouts[index] = vkDevice.createDescriptorSetLayout(info);
	}

	vk::PushConstantRange range = {};

	if (mConstant32Bits.has_value()) {
		range.offset = 0;
		range.size = static_cast<uint32_t>(mConstant32Bits->Count * sizeof(UInt32));
		range.stageFlags = enumConvert(mConstant32Bits->Visibility);
	}
	
	//create pipeline layout
	vk::PipelineLayoutCreateInfo layoutInfo = {};

	layoutInfo
		.setPNext(nullptr)
		.setFlags(vk::PipelineLayoutCreateFlags(0))
		.setSetLayoutCount(static_cast<uint32_t>(mDescriptorSetLayouts.size()))
		.setPushConstantRangeCount(mConstant32Bits.has_value() ? 1 : 0)
		.setPSetLayouts(mDescriptorSetLayouts.data())
		.setPPushConstantRanges(&range);

	mPipelineLayout = vkDevice.createPipelineLayout(layoutInfo);
}

CodeRed::VulkanResourceLayout::~VulkanResourceLayout()
{
	const auto vkDevice = std::static_pointer_cast<VulkanLogicalDevice>(mDevice)->device();
	
	vkDevice.destroyPipelineLayout(mPipelineLayout);

	for (auto& setLayout : mDescriptorSetLayouts) 
		vkDevice.destroyDescriptorSetLayout(setLayout);
}

#endif
