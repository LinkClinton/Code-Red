#include "../../Shared/PixelFormatSizeOf.hpp"
#include "VulkanInputAssemblyState.hpp"

#ifdef __ENABLE__VULKAN__

using namespace CodeRed::Vulkan;

CodeRed::VulkanInputAssemblyState::VulkanInputAssemblyState(
	const std::shared_ptr<GpuLogicalDevice>& device,
	const std::vector<InputLayoutElement>& elements,
	const PrimitiveTopology primitive_topology) :
	GpuInputAssemblyState(device, elements, primitive_topology)
{
	mVertexBinding.binding = 0;
	mVertexBinding.inputRate = vk::VertexInputRate::eVertex;
	mVertexBinding.stride = 0;

	for (size_t index = 0; index < mElements.size(); index++) {
		mVertexAttributes.push_back(
			vk::VertexInputAttributeDescription(
				static_cast<uint32_t>(index), 0,
				enumConvert(elements[index].Format),
				mVertexBinding.stride)
		);

		mVertexBinding.stride = static_cast<uint32_t>(mVertexBinding.stride + PixelFormatSizeOf::get(elements[index].Format));
	}

	mVertexInput
		.setPNext(nullptr)
		.setFlags(vk::PipelineVertexInputStateCreateFlags(0))
		.setVertexBindingDescriptionCount(1)
		.setVertexAttributeDescriptionCount(static_cast<uint32_t>(mVertexAttributes.size()))
		.setPVertexBindingDescriptions(&mVertexBinding)
		.setPVertexAttributeDescriptions(mVertexAttributes.empty() ? nullptr : mVertexAttributes.data());

	mInputAssembly
		.setPNext(nullptr)
		.setFlags(vk::PipelineInputAssemblyStateCreateFlags(0))
		.setPrimitiveRestartEnable(false)
		.setTopology(enumConvert(mPrimitiveTopology));
}


#endif
