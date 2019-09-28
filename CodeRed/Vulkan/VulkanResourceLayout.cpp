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
	const size_t maxBindResources) :
	GpuResourceLayout(device, elements, samplers, maxBindResources)
{
	size_t maxSpace = 0;
	
	//find the max space we use, then we will create layouts with maxSpace
	for (auto element : elements) maxSpace = std::max(maxSpace, element.Space + 1);
	for (auto sampler : samplers) maxSpace = std::max(maxSpace, sampler.Space + 1);

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
			.setBindingCount(static_cast<uint32_t>(bindings[index].size()))
			.setPBindings(bindings[index].data());

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
	vk::DescriptorPoolSize poolSizes[2];

	poolSizes[0].descriptorCount = static_cast<uint32_t>(mMaxBindResources);
	poolSizes[1].descriptorCount = static_cast<uint32_t>(mMaxBindResources);
	poolSizes[0].type = vk::DescriptorType::eUniformBuffer;
	poolSizes[1].type = vk::DescriptorType::eSampledImage;
	
	poolInfo
		.setPNext(nullptr)
		.setFlags(vk::DescriptorPoolCreateFlags(0))
		.setMaxSets(static_cast<uint32_t>(maxSpace))
		.setPoolSizeCount(2)
		.setPPoolSizes(poolSizes);

	mDescriptorPool = vkDevice.createDescriptorPool(poolInfo);

	mWriteDescriptorSets = std::vector<vk::WriteDescriptorSet>(mElements.size());

	for (size_t index = 0; index < mWriteDescriptorSets.size(); index++) {
		mWriteDescriptorSets[index]
			.setPNext(nullptr)
			.setDescriptorCount(1)
			.setDstArrayElement(0)
			.setDstBinding(static_cast<uint32_t>(mDescriptorBindings[index]))
			.setDescriptorType(enumConvert(mElements[index].Type));
	}
}

CodeRed::VulkanResourceLayout::~VulkanResourceLayout()
{
	const auto vkDevice = std::static_pointer_cast<VulkanLogicalDevice>(mDevice)->device();

	for (auto it : mDescriptors) 
		vkDevice.freeDescriptorSets(mDescriptorPool, it.second.first);
	
	vkDevice.destroyPipelineLayout(mPipelineLayout);
	vkDevice.destroyDescriptorPool(mDescriptorPool);

	for (auto& setLayout : mDescriptorSetLayouts) 
		vkDevice.destroyDescriptorSetLayout(setLayout);
}

void CodeRed::VulkanResourceLayout::reset()
{
	const auto vkDevice = std::static_pointer_cast<VulkanLogicalDevice>(mDevice)->device();
	
	for (auto it : mDescriptors)
		vkDevice.freeDescriptorSets(mDescriptorPool, it.second.first);

	mDescriptors.clear();
}

void CodeRed::VulkanResourceLayout::bindTexture(
	const size_t index,
	const std::shared_ptr<GpuTexture>& resource)
{
	const auto it = mDescriptors.find(resource);
	
	//the resource is bound, so we do not need to bind again
	if (it != mDescriptors.end() && it->second.second == index) return;

	CODE_RED_DEBUG_THROW_IF(
		mDescriptors.size() >= mMaxBindResources && it == mDescriptors.end(),
		Exception(
			"Bind resource to the resource layout failed."
			"Because the number of resources are too many.")
	);
	
	CODE_RED_DEBUG_THROW_IF(
		index >= mElements.size(),
		InvalidException<size_t>({ "index" })
	);

	CODE_RED_DEBUG_THROW_IF(
		mElements[index].Type != ResourceType::Texture,
		InvalidException<ResourceType>({ "element(index).Type" })
	);
	
	const auto vkDevice = std::static_pointer_cast<VulkanLogicalDevice>(mDevice)->device();
	const auto texture = std::static_pointer_cast<VulkanTexture>(resource);

	vk::DescriptorSetAllocateInfo info = {};

	info
		.setPNext(nullptr)
		.setDescriptorPool(mDescriptorPool)
		.setDescriptorSetCount(1)
		.setPSetLayouts(&mDescriptorSetLayouts[mElements[index].Space]);

	//the descriptor is existed, we need update it, so we free the old descriptor
	if (it != mDescriptors.end()) vkDevice.freeDescriptorSets(mDescriptorPool, it->second.first);

	const auto bindDescriptor = mDescriptors[resource] = std::make_pair(vkDevice.allocateDescriptorSets(info)[0], index);

	vk::DescriptorImageInfo imageInfo = {};

	imageInfo
		.setImageLayout(enumConvert(texture->layout()))
		.setImageView(texture->view())
		.setSampler(nullptr);
	
	mWriteDescriptorSets[index]
		.setDstSet(bindDescriptor.first)
		.setPImageInfo(&imageInfo);

	vkDevice.updateDescriptorSets(mWriteDescriptorSets[index], {});
}

void CodeRed::VulkanResourceLayout::bindBuffer(
	const size_t index, 
	const std::shared_ptr<GpuBuffer>& resource)
{
	const auto it = mDescriptors.find(resource);

	//the resource is bound, so we do not need to bind again
	if (it != mDescriptors.end() && it->second.second == index) return;

	CODE_RED_DEBUG_THROW_IF(
		mDescriptors.size() >= mMaxBindResources && it == mDescriptors.end(),
		Exception(
			"Bind resource to the resource layout failed."
			"Because the number of resources are too many.")
	);

	CODE_RED_DEBUG_THROW_IF(
		index >= mElements.size(),
		InvalidException<size_t>({ "index" })
	);

	CODE_RED_DEBUG_THROW_IF(
		mElements[index].Type != ResourceType::Buffer,
		InvalidException<ResourceType>({ "element(index).Type" })
	);

	const auto vkDevice = std::static_pointer_cast<VulkanLogicalDevice>(mDevice)->device();
	const auto buffer = std::static_pointer_cast<VulkanBuffer>(resource);

	vk::DescriptorSetAllocateInfo info = {};

	info
		.setPNext(nullptr)
		.setDescriptorPool(mDescriptorPool)
		.setDescriptorSetCount(1)
		.setPSetLayouts(&mDescriptorSetLayouts[mElements[index].Space]);

	//the descriptor is existed, we need update it, so we free the old descriptor
	if (it != mDescriptors.end()) vkDevice.freeDescriptorSets(mDescriptorPool, it->second.first);

	const auto bindDescriptor = mDescriptors[resource] = std::make_pair(vkDevice.allocateDescriptorSets(info)[0], index);

	vk::DescriptorBufferInfo bufferInfo = {};

	bufferInfo
		.setBuffer(buffer->buffer())
		.setOffset(0)
		.setRange(VK_WHOLE_SIZE);

	mWriteDescriptorSets[index]
		.setDstSet(bindDescriptor.first)
		.setPBufferInfo(&bufferInfo);

	vkDevice.updateDescriptorSets(mWriteDescriptorSets[index], {});
}

void CodeRed::VulkanResourceLayout::unbindResource(const std::shared_ptr<GpuResource>& resource)
{
	const auto it = mDescriptors.find(resource);

	CODE_RED_DEBUG_THROW_IF(
		it == mDescriptors.end(),
		Exception(
			"Unbind resource from resource layout failed."
			"Because the resource is not bound to the resource layout.")
	);

	if (it == mDescriptors.end()) return;

	const auto vkDevice = std::static_pointer_cast<VulkanLogicalDevice>(mDevice)->device();

	vkDevice.freeDescriptorSets(mDescriptorPool, it->second.first);

	mDescriptors.erase(it);
}

auto CodeRed::VulkanResourceLayout::descriptor(
	const std::shared_ptr<GpuResource>& resource)
	-> vk::DescriptorSet
{
	CODE_RED_DEBUG_THROW_IF(
		mDescriptors.find(resource) == mDescriptors.end(),
		FailedException({ "Descriptor", "ResourceLayout" }, DebugType::Get)
	);
	
	return mDescriptors[resource].first;
}

#endif
