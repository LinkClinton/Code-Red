#include "VulkanResourceLayout.hpp"
#include "VulkanLogicalDevice.hpp"

#ifdef __ENABLE__VULKAN__
#undef max

CodeRed::VulkanResourceLayout::VulkanResourceLayout(
	const std::shared_ptr<GpuLogicalDevice>& device,
	const std::vector<ResourceLayoutElement>& elements, 
	const std::vector<SamplerLayoutElement>& samplers) :
	GpuResourceLayout(device, elements, samplers)
{
	size_t maxSpace = 0;

	for (auto element : elements) maxSpace = std::max(maxSpace, element.Space);
	for (auto sampler : samplers) maxSpace = std::max(maxSpace, sampler.Space);

	std::vector<std::vector<vk::DescriptorSetLayoutBinding>> bindings(maxSpace);

	for (const auto element : elements) {
		vk::DescriptorSetLayoutBinding binding = {};

		binding
			.setBinding(element.Binding)
			.setDescriptorType(enumConvert(element.Type))
			.setDescriptorCount(1)
			.setStageFlags(enumConvert(element.Visibility))
			.setPImmutableSamplers(nullptr);

		bindings[element.Space].push_back(binding);
	}

	for (const auto sampler : samplers) {
		vk::DescriptorSetLayoutBinding binding = {};

		binding
			.setBinding(sampler.Binding)
			.setDescriptorType(vk::DescriptorType::eSampler)
			.setDescriptorCount(1)
			.setStageFlags(enumConvert(sampler.Visibility))
			.setPImmutableSamplers();

		bindings[sampler.Space].push_back(binding);
	}
}

#endif