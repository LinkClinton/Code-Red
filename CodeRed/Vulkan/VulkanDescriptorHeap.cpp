#include "../Shared/Exception/InvalidException.hpp"

#include "VulkanResource/VulkanTexture.hpp"
#include "VulkanResource/VulkanBuffer.hpp"

#include "VulkanDescriptorHeap.hpp"
#include "VulkanResourceLayout.hpp"
#include "VulkanLogicalDevice.hpp"

#ifdef __ENABLE__VULKAN__

using namespace CodeRed::Vulkan;

CodeRed::VulkanDescriptorHeap::VulkanDescriptorHeap(
	const std::shared_ptr<GpuLogicalDevice>& device,
	const std::shared_ptr<GpuResourceLayout>& layout) :
	GpuDescriptorHeap(device, layout)
{
	const auto vkDevice = std::static_pointer_cast<VulkanLogicalDevice>(mDevice)->device();
	const auto vkLayout = std::static_pointer_cast<VulkanResourceLayout>(mResourceLayout);
	
	uint32_t bufferCount = 0;
	uint32_t textureCount = 0;

	for (const auto element : vkLayout->mElements) {
		bufferCount += element.Type == ResourceType::Buffer ? 1 : 0;
		textureCount += element.Type == ResourceType::Texture ? 1 : 0;
	}

	std::vector<vk::DescriptorPoolSize> poolSize;

	CODE_RED_TRY_EXECUTE(
		bufferCount > 0,
		poolSize.push_back({ vk::DescriptorType::eUniformBuffer, bufferCount })
	);

	CODE_RED_TRY_EXECUTE(
		textureCount > 0,
		poolSize.push_back({ vk::DescriptorType::eSampledImage, textureCount })
	);

	CODE_RED_TRY_EXECUTE(
		vkLayout->mSamplers.empty() == false,
		poolSize.push_back({ vk::DescriptorType::eSampler, static_cast<uint32_t>(vkLayout->mSamplers.size()) })
	);
	
	vk::DescriptorPoolCreateInfo info = {};

	info
		.setPNext(nullptr)
		.setFlags(vk::DescriptorPoolCreateFlagBits::eFreeDescriptorSet)
		.setMaxSets(static_cast<uint32_t>(vkLayout->mDescriptorSetLayouts.size()))
		.setPoolSizeCount(static_cast<uint32_t>(poolSize.size()))
		.setPPoolSizes(poolSize.data());

	mDescriptorPool = vkDevice.createDescriptorPool(info);

	vk::DescriptorSetAllocateInfo allocateInfo = {};

	allocateInfo
		.setPNext(nullptr)
		.setDescriptorPool(mDescriptorPool)
		.setDescriptorSetCount(static_cast<uint32_t>(vkLayout->mDescriptorSetLayouts.size()))
		.setPSetLayouts(vkLayout->mDescriptorSetLayouts.data());

	mDescriptorSets = vkDevice.allocateDescriptorSets(allocateInfo);
}

CodeRed::VulkanDescriptorHeap::~VulkanDescriptorHeap()
{
	const auto vkDevice = std::static_pointer_cast<VulkanLogicalDevice>(mDevice)->device();
	
	for (auto& descriptorSet : mDescriptorSets)
		vkDevice.freeDescriptorSets(mDescriptorPool, mDescriptorSets);

	vkDevice.destroyDescriptorPool(mDescriptorPool);
}

void CodeRed::VulkanDescriptorHeap::bindTexture(
	const size_t index, 
	const std::shared_ptr<GpuTexture>& texture)
{
	CODE_RED_DEBUG_THROW_IF(
		index >= mResourceLayout->mElements.size(),
		InvalidException<size_t>({ "index" })
	);

	CODE_RED_DEBUG_THROW_IF(
		mResourceLayout->mElements[index].Type != ResourceType::Texture,
		InvalidException<ResourceType>({ "element(index).Type" })
	);

	vk::DescriptorImageInfo imageInfo = {};
	vk::WriteDescriptorSet write = {};

	imageInfo
		.setImageLayout(enumConvert(texture->layout()))
		.setImageView(std::static_pointer_cast<VulkanTexture>(texture)->view())
		.setSampler(nullptr);
	
	write
		.setPNext(nullptr)
		.setDescriptorCount(1)
		.setDescriptorType(enumConvert(texture->type()))
		.setDstArrayElement(0)
		.setDstBinding(static_cast<uint32_t>(mResourceLayout->mElements[index].Binding))
		.setDstSet(mDescriptorSets[mResourceLayout->mElements[index].Space])
		.setPImageInfo(&imageInfo);

	const auto vkDevice = std::static_pointer_cast<VulkanLogicalDevice>(mDevice)->device();
	
	vkDevice.updateDescriptorSets(write, {});
}

void CodeRed::VulkanDescriptorHeap::bindBuffer(
	const size_t index, 
	const std::shared_ptr<GpuBuffer>& buffer)
{
	CODE_RED_DEBUG_THROW_IF(
		index >= mResourceLayout->mElements.size(),
		InvalidException<size_t>({ "index" })
	);

	CODE_RED_DEBUG_THROW_IF(
		mResourceLayout->mElements[index].Type != ResourceType::Buffer,
		InvalidException<ResourceType>({ "element(index).Type" })
	);

	vk::DescriptorBufferInfo bufferInfo = {};
	vk::WriteDescriptorSet write = {};

	bufferInfo
		.setOffset(0)
		.setRange(buffer->size())
		.setBuffer(std::static_pointer_cast<VulkanBuffer>(buffer)->buffer());

	write
		.setPNext(nullptr)
		.setDescriptorCount(1)
		.setDescriptorType(enumConvert(buffer->type()))
		.setDstArrayElement(0)
		.setDstBinding(static_cast<uint32_t>(mResourceLayout->mElements[index].Binding))
		.setDstSet(mDescriptorSets[mResourceLayout->mElements[index].Space])
		.setPBufferInfo(&bufferInfo);

	const auto vkDevice = std::static_pointer_cast<VulkanLogicalDevice>(mDevice)->device();

	vkDevice.updateDescriptorSets(write, {});
}

#endif
