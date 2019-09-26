#include "../Shared/Exception/InvalidException.hpp"

#include "VulkanResource/VulkanTexture.hpp"
#include "VulkanResource/VulkanSampler.hpp"
#include "VulkanResource/VulkanBuffer.hpp"

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
	size_t bufferCount = 0;
	size_t textureCount = 0;

	//find the max space we use, then we will create layouts with maxSpace
	for (auto element : elements) maxSpace = std::max(maxSpace, element.Space);
	for (auto sampler : samplers) maxSpace = std::max(maxSpace, sampler.Space);

	std::vector<std::vector<vk::DescriptorSetLayoutBinding>> bindings(maxSpace);

	//generate the binding information of elements
	for (const auto element : mElements) {
		vk::DescriptorSetLayoutBinding binding = {};

		binding
			.setBinding(static_cast<uint32_t>(element.Binding))
			.setDescriptorType(enumConvert(element.Type))
			.setDescriptorCount(1)
			.setStageFlags(enumConvert(element.Visibility))
			.setPImmutableSamplers(nullptr);

		bindings[element.Space].push_back(binding);

		bufferCount = bufferCount + (element.Type == ResourceType::Buffer ? 1 : 0);
		textureCount = textureCount + (element.Type == ResourceType::Texture ? 1 : 0);

		mDescriptorBindings.push_back(bindings[element.Space].size() - 1);
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

		mDescriptorBindings.push_back(bindings[sampler.Space].size() - 1);
	}

	const auto vkDevice = std::static_pointer_cast<VulkanLogicalDevice>(mDevice)->device();

	mDescriptorSetLayouts = std::vector<vk::DescriptorSetLayout>(maxSpace);

	//create the set layouts with bindings
	for (size_t index = 0; index < mDescriptorSetLayouts.size(); index++) {
		vk::DescriptorSetLayoutCreateInfo info = {};

		info
			.setPNext(nullptr)
			.setFlags(vk::DescriptorSetLayoutCreateFlags(0))
			.setBindingCount(static_cast<uint32_t>(bindings[index].size()));

		mDescriptorSetLayouts[index] = vkDevice.createDescriptorSetLayout(info);
	}

	//create pipeline layout
	vk::PipelineLayoutCreateInfo layoutInfo = {};

	layoutInfo
		.setPNext(nullptr)
		.setFlags(vk::PipelineLayoutCreateFlags(0))
		.setSetLayoutCount(static_cast<uint32_t>(mDescriptorSetLayouts.size()))
		.setPushConstantRangeCount(0)
		.setPSetLayouts(mDescriptorSetLayouts.data())
		.setPPushConstantRanges(nullptr);

	mPipelineLayout = vkDevice.createPipelineLayout(layoutInfo);

	//create descriptor pool for this resource layout
	vk::DescriptorPoolCreateInfo poolInfo = {};
	vk::DescriptorPoolSize poolSizes[3];

	poolSizes[0].descriptorCount = static_cast<uint32_t>(bufferCount);
	poolSizes[1].descriptorCount = static_cast<uint32_t>(textureCount);
	poolSizes[2].descriptorCount = static_cast<uint32_t>(mSamplers.size());
	poolSizes[0].type = vk::DescriptorType::eUniformBuffer;
	poolSizes[1].type = vk::DescriptorType::eSampledImage;
	poolSizes[2].type = vk::DescriptorType::eSampler;

	poolInfo
		.setPNext(nullptr)
		.setFlags(vk::DescriptorPoolCreateFlags(0))
		.setMaxSets(static_cast<uint32_t>(maxSpace))
		.setPoolSizeCount(3)
		.setPPoolSizes(poolSizes);

	mDescriptorPool = vkDevice.createDescriptorPool(poolInfo);

	std::vector<vk::DescriptorSetLayout> layouts;
	vk::DescriptorSetAllocateInfo allocateInfo = {};
	
	//prepare the set layouts for allocate descriptor sets
	for (const auto element : mElements) layouts.push_back(mDescriptorSetLayouts[element.Space]);
	for (const auto sampler : mSamplers) layouts.push_back(mDescriptorSetLayouts[sampler.Space]);

	allocateInfo
		.setPNext(nullptr)
		.setDescriptorPool(mDescriptorPool)
		.setDescriptorSetCount(static_cast<uint32_t>(layouts.size()))
		.setPSetLayouts(layouts.data());

	mDescriptorSets = vkDevice.allocateDescriptorSets(allocateInfo);

	mWriteDescriptorSets = std::vector<vk::WriteDescriptorSet>(mDescriptorSets.size());

	for (size_t index = 0; index < mWriteDescriptorSets.size(); index++) {
		mWriteDescriptorSets[index]
			.setPNext(nullptr)
			.setDstSet(mDescriptorSets[index])
			.setDescriptorCount(1)
			.setDstArrayElement(0)
			.setDstBinding(static_cast<uint32_t>(mDescriptorBindings[index]));

		mWriteDescriptorSets[index].descriptorType =
			index < mElements.size() ?
			enumConvert(mElements[index].Type) :
			vk::DescriptorType::eSampler;
	}
}

CodeRed::VulkanResourceLayout::~VulkanResourceLayout()
{
	const auto vkDevice = std::static_pointer_cast<VulkanLogicalDevice>(mDevice)->device();

	vkDevice.freeDescriptorSets(mDescriptorPool, mDescriptorSets);
	vkDevice.destroyPipelineLayout(mPipelineLayout);
	vkDevice.destroyDescriptorPool(mDescriptorPool);

	for (auto& setLayout : mDescriptorSetLayouts) 
		vkDevice.destroyDescriptorSetLayout(setLayout);
}

void CodeRed::VulkanResourceLayout::bindTexture(
	const size_t index, 
	const std::shared_ptr<GpuTexture>& resource)
{
	CODE_RED_DEBUG_THROW_IF(
		index >= mElements.size(),
		InvalidException<size_t>({ "index" })
	);

	CODE_RED_DEBUG_THROW_IF(
		mElements[index].Type != ResourceType::Texture ||
		resource->type() != ResourceType::Texture,
		InvalidException<ResourceType>({ "texture.type()" })
	);

	vk::DescriptorImageInfo imageInfo = {};

	imageInfo
		.setImageLayout(enumConvert(resource->layout()))
		.setImageView(std::static_pointer_cast<VulkanTexture>(resource)->view())
		.setSampler(nullptr);
	
	auto& write = mWriteDescriptorSets[index];

	write
		.setDescriptorType(enumConvert(resource->type()))
		.setPImageInfo(&imageInfo);

	const auto vkDevice = std::static_pointer_cast<VulkanLogicalDevice>(mDevice)->device();

	vkDevice.updateDescriptorSets(1, &mWriteDescriptorSets[index], 0, nullptr);
}

void CodeRed::VulkanResourceLayout::bindBuffer(
	const size_t index, 
	const std::shared_ptr<GpuBuffer>& resource)
{
	CODE_RED_DEBUG_THROW_IF(
		index >= mElements.size(),
		InvalidException<size_t>({ "index" })
	);

	CODE_RED_DEBUG_THROW_IF(
		mElements[index].Type != ResourceType::Buffer ||
		resource->type() != ResourceType::Buffer,
		InvalidException<ResourceType>({ "resource.type()" })
	);

	vk::DescriptorBufferInfo bufferInfo = {};

	bufferInfo
		.setBuffer(std::static_pointer_cast<VulkanBuffer>(resource)->buffer())
		.setOffset(0)
		.setRange(resource->size());

	auto& write = mWriteDescriptorSets[index];

	write
		.setDescriptorType(enumConvert(resource->type()))
		.setPBufferInfo(&bufferInfo);

	const auto vkDevice = std::static_pointer_cast<VulkanLogicalDevice>(mDevice)->device();

	vkDevice.updateDescriptorSets(1, &mWriteDescriptorSets[index], 0, nullptr);
}

#endif
