#include "../Shared/Exception/NotSupportException.hpp"
#include "../Shared/Exception/InvalidException.hpp"

#include "../Shared/Enum/PrimitiveTopology.hpp"
#include "../Shared/Enum/ShaderVisibility.hpp"
#include "../Shared/Enum/CompareOperator.hpp"
#include "../Shared/Enum/StencilOperator.hpp"
#include "../Shared/Enum/ResourceLayout.hpp"
#include "../Shared/Enum/FilterOptions.hpp"
#include "../Shared/Enum/ResourceUsage.hpp"
#include "../Shared/Enum/BlendOperator.hpp"
#include "../Shared/Enum/ResourceType.hpp"
#include "../Shared/Enum/PixelFormat.hpp"
#include "../Shared/Enum/AddressMode.hpp"
#include "../Shared/Enum/BorderColor.hpp"
#include "../Shared/Enum/BlendFactor.hpp"
#include "../Shared/Enum/MemoryHeap.hpp"
#include "../Shared/Enum/Dimension.hpp"
#include "../Shared/Enum/FillMode.hpp"
#include "../Shared/Enum/CullMode.hpp"

#include "VulkanUtility.hpp"

#ifdef __ENABLE__VULKAN__

#pragma comment(lib, "vulkan-1.lib")

auto CodeRed::enumConvert(const FilterOptions filter, const size_t index)
	-> vk::Filter
{
	const UInt32 mask[] = {
		4, //100, mig
		2, //010, mag
		1 //001, mip
	};

	return (static_cast<UInt32>(filter) & mask[index]) != 0 ? vk::Filter::eLinear : vk::Filter::eNearest;
}

auto CodeRed::enumConvert(const FilterOptions filter)
	-> vk::SamplerMipmapMode
{
	return (static_cast<UInt32>(filter) & 1) != 0 ?
		vk::SamplerMipmapMode::eLinear :
		vk::SamplerMipmapMode::eNearest;
}

auto CodeRed::enumConvert(const AddressMode mode)
	-> vk::SamplerAddressMode
{
	switch (mode) {
	case AddressMode::Repeat: return vk::SamplerAddressMode::eRepeat;
	case AddressMode::Mirror: return vk::SamplerAddressMode::eMirroredRepeat;
	case AddressMode::Clamp: return vk::SamplerAddressMode::eClampToEdge;
	case AddressMode::Border: return vk::SamplerAddressMode::eClampToBorder;
	default:
		throw NotSupportException(NotSupportType::Enum);
	}
}

auto CodeRed::enumConvert(const BorderColor color)
	-> vk::BorderColor
{
	switch (color) {
	case BorderColor::TransparentBlack: return vk::BorderColor::eFloatTransparentBlack;
	case BorderColor::OpaqueBlack: return vk::BorderColor::eFloatOpaqueBlack;
	case BorderColor::OpaqueWhite: return vk::BorderColor::eFloatOpaqueWhite;
	default:
		throw NotSupportException(NotSupportType::Enum);
	}
}

auto CodeRed::enumConvert(const ResourceType type)
	-> vk::DescriptorType
{
	switch (type) {
	case ResourceType::Buffer: return vk::DescriptorType::eUniformBuffer;
	case ResourceType::Texture: return vk::DescriptorType::eSampledImage;
	default:
		throw NotSupportException(NotSupportType::Enum);
	}
}

auto CodeRed::enumConvert(const ShaderVisibility visibility)
	-> vk::ShaderStageFlagBits
{
	switch (visibility) {
	case ShaderVisibility::All: return vk::ShaderStageFlagBits::eAll;
	case ShaderVisibility::Vertex: return vk::ShaderStageFlagBits::eVertex;
	case ShaderVisibility::Pixel: return vk::ShaderStageFlagBits::eFragment;
	default:
		throw NotSupportException(NotSupportType::Enum);
	}
}

#endif
