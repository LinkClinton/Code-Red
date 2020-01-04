#include "../../Shared/PixelFormatSizeOf.hpp"
#include "VulkanInputAssemblyState.hpp"

#ifdef __ENABLE__VULKAN__
#undef max

using namespace CodeRed::Vulkan;

CodeRed::VulkanInputAssemblyState::VulkanInputAssemblyState(
	const std::shared_ptr<GpuLogicalDevice>& device,
	const std::vector<InputLayoutElement>& elements,
	const PrimitiveTopology primitive_topology) :
	GpuInputAssemblyState(device, elements, primitive_topology)
{
	mVertexBindings = std::vector<vk::VertexInputBindingDescription>(mSlotCount);
	
	for (size_t index = 0; index < mElements.size(); index++) {
		const auto slot = mElements[index].Slot;
		
		mVertexAttributes.push_back(
			vk::VertexInputAttributeDescription(
				static_cast<uint32_t>(index), 
				static_cast<uint32_t>(slot),
				enumConvert(mElements[index].Format),
				mVertexBindings[slot].stride)
		);

		mVertexBindings[slot].stride = 
			static_cast<uint32_t>(mVertexBindings[slot].stride + PixelFormatSizeOf::get(mElements[index].Format));
	}

	for (size_t index = 0; index < mVertexBindings.size(); index++) 
		mVertexBindings[index].binding = static_cast<uint32_t>(index);

	mVertexInput
		.setPNext(nullptr)
		.setFlags(vk::PipelineVertexInputStateCreateFlags(0))
		.setVertexBindingDescriptionCount(static_cast<uint32_t>(mVertexBindings.size()))
		.setVertexAttributeDescriptionCount(static_cast<uint32_t>(mVertexAttributes.size()))
		.setPVertexBindingDescriptions(mVertexBindings.empty() ? nullptr : mVertexBindings.data())
		.setPVertexAttributeDescriptions(mVertexAttributes.empty() ? nullptr : mVertexAttributes.data());

	mInputAssembly
		.setPNext(nullptr)
		.setFlags(vk::PipelineInputAssemblyStateCreateFlags(0))
		.setPrimitiveRestartEnable(false)
		.setTopology(enumConvert(mPrimitiveTopology));
}


#endif
