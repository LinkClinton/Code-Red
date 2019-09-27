#include "../Shared/Exception/NotSupportException.hpp"
#include "../Shared/Exception/InvalidException.hpp"

#include "../Shared/Enum/PrimitiveTopology.hpp"
#include "../Shared/Enum/ShaderVisibility.hpp"
#include "../Shared/Enum/CompareOperator.hpp"
#include "../Shared/Enum/StencilOperator.hpp"
#include "../Shared/Enum/AttachmentStore.hpp"
#include "../Shared/Enum/ResourceLayout.hpp"
#include "../Shared/Enum/AttachmentLoad.hpp"
#include "../Shared/Enum/FilterOptions.hpp"
#include "../Shared/Enum/ResourceUsage.hpp"
#include "../Shared/Enum/BlendOperator.hpp"
#include "../Shared/Enum/ResourceType.hpp"
#include "../Shared/Enum/PixelFormat.hpp"
#include "../Shared/Enum/AddressMode.hpp"
#include "../Shared/Enum/BorderColor.hpp"
#include "../Shared/Enum/BlendFactor.hpp"
#include "../Shared/Enum/ShaderType.hpp"
#include "../Shared/Enum/MemoryHeap.hpp"
#include "../Shared/Enum/FrontFace.hpp"
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

auto CodeRed::enumConvert(const ResourceLayout layout)
	-> vk::ImageLayout
{
	switch (layout) {
	case ResourceLayout::GeneralRead: return vk::ImageLayout::eGeneral;
	case ResourceLayout::RenderTarget: return vk::ImageLayout::eColorAttachmentOptimal;
	case ResourceLayout::DepthStencil: return vk::ImageLayout::eDepthStencilAttachmentOptimal;
	case ResourceLayout::CopyDestination: return vk::ImageLayout::eTransferDstOptimal;
	case ResourceLayout::CopySource: return vk::ImageLayout::eTransferSrcOptimal;
	case ResourceLayout::Present: return vk::ImageLayout::ePresentSrcKHR;
	default:
		throw NotSupportException(NotSupportType::Enum);
	}
}

auto CodeRed::enumConvert(const ResourceUsage usage)
	-> VulkanResourceUsage
{
	//only run with macro __ENABLE_CODE_RED_DEBUG__
	//This macro is used to enable internal debug in lib
#ifdef __ENABLE__CODE__RED__DEBUG__

	//we record all masks that we can not combine
	static ResourceUsage disableMask[] = {
		ResourceUsage::RenderTarget | ResourceUsage::DepthStencil,
		ResourceUsage::VertexBuffer | ResourceUsage::RenderTarget,
		ResourceUsage::VertexBuffer | ResourceUsage::DepthStencil,
		ResourceUsage::IndexBuffer | ResourceUsage::RenderTarget,
		ResourceUsage::IndexBuffer | ResourceUsage::DepthStencil,
		ResourceUsage::ConstantBuffer | ResourceUsage::RenderTarget,
		ResourceUsage::ConstantBuffer | ResourceUsage::DepthStencil
	};

	//if usage has this mask we disable, we will throw a InvalidException with nullptr
	for (auto mask : disableMask) {
		if (enumHas(usage, mask))
			throw InvalidException<ResourceUsage>({ "usage" });
	}
#endif
	
	static std::vector<ResourceUsage> sourcePool = {
		ResourceUsage::None,
		ResourceUsage::VertexBuffer,
		ResourceUsage::IndexBuffer,
		ResourceUsage::ConstantBuffer,
		ResourceUsage::RenderTarget,
		ResourceUsage::DepthStencil
	};

	static std::vector<VulkanResourceUsage> targetPool = {
		VulkanResourceUsage(0, 0) ,
		VulkanResourceUsage(vk::BufferUsageFlagBits::eVertexBuffer, 0),
		VulkanResourceUsage(vk::BufferUsageFlagBits::eIndexBuffer, 0),
		VulkanResourceUsage(vk::BufferUsageFlagBits::eUniformBuffer, 0),
		VulkanResourceUsage(0, vk::ImageUsageFlagBits::eColorAttachment),
		VulkanResourceUsage(0, vk::ImageUsageFlagBits::eDepthStencilAttachment)
	};

	auto res = VulkanResourceUsage(0, 0);

	for (size_t index = 0; index < sourcePool.size(); index++) {
		if (enumHas(usage, sourcePool[index]))
			res.first = res.first | targetPool[index].first,
			res.second = res.second | targetPool[index].second;
	}

	return res;
}

auto CodeRed::enumConvert(const MemoryHeap heap)
	-> vk::MemoryPropertyFlags
{
	switch (heap) {
	case MemoryHeap::Default: return vk::MemoryPropertyFlagBits::eDeviceLocal;
	case MemoryHeap::Upload: return vk::MemoryPropertyFlagBits::eHostVisible | 
		vk::MemoryPropertyFlagBits::eHostCoherent;
	default:
		throw NotSupportException(NotSupportType::Enum);
	}
}

auto CodeRed::enumConvert(const PixelFormat format)
	-> vk::Format
{
	switch (format) {
	case PixelFormat::RedGreenBlueAlpha8BitUnknown: return vk::Format::eR8G8B8A8Unorm;
	case PixelFormat::RedGreenBlueAlpha32BitFloat: return vk::Format::eR32G32B32A32Sfloat;
	case PixelFormat::RedGreenBlue32BitFloat: return vk::Format::eR32G32B32Sfloat;
	case PixelFormat::RedGreen32BitFloat: return vk::Format::eR32G32Sfloat;
	case PixelFormat::Unknown: return vk::Format::eUndefined;
	default:
		throw NotSupportException(NotSupportType::Enum);
	}
}

auto CodeRed::enumConvert(const Dimension dimension)
	-> vk::ImageType
{
	switch (dimension) {
	case Dimension::Dimension1D: return vk::ImageType::e1D;
	case Dimension::Dimension2D: return vk::ImageType::e2D;
	case Dimension::Dimension3D: return vk::ImageType::e3D;
	default:
		throw NotSupportException(NotSupportType::Enum);
	}
}

auto CodeRed::enumConvert(const ColorMask mask)
	-> vk::ColorComponentFlags
{
	//the enum is same as vk::ColorComponentFlags
	//so we only change the type
	return vk::ColorComponentFlags(static_cast<uint32_t>(mask));
}

auto CodeRed::enumConvert(const BlendOperator op)
	-> vk::BlendOp
{
	switch (op) {
	case BlendOperator::Add: return vk::BlendOp::eAdd;
	case BlendOperator::Subtract: return vk::BlendOp::eSubtract;
	case BlendOperator::ReverseSubtract: return vk::BlendOp::eReverseSubtract;
	case BlendOperator::Min: return vk::BlendOp::eMin;
	case BlendOperator::Max: return vk::BlendOp::eMax;
	default:
		throw NotSupportException(NotSupportType::Enum);
	}
}

auto CodeRed::enumConvert(const BlendFactor factor)
	-> vk::BlendFactor
{
	switch (factor) {
	case BlendFactor::Zero: return vk::BlendFactor::eZero;
	case BlendFactor::One: return vk::BlendFactor::eOne;
	case BlendFactor::SrcColor: return vk::BlendFactor::eSrcColor;
	case BlendFactor::InvSrcColor: return vk::BlendFactor::eOneMinusSrcColor;
	case BlendFactor::SrcAlpha: return vk::BlendFactor::eSrcAlpha;
	case BlendFactor::InvSrcAlpha: return vk::BlendFactor::eOneMinusSrcAlpha;
	case BlendFactor::DestAlpha: return vk::BlendFactor::eDstAlpha;
	case BlendFactor::InvDestAlpha: return vk::BlendFactor::eOneMinusDstAlpha;
	case BlendFactor::DescColor: return vk::BlendFactor::eDstColor;
	case BlendFactor::InvDestColor: return vk::BlendFactor::eOneMinusDstColor;
	case BlendFactor::Factor: return vk::BlendFactor::eConstantColor;
	case BlendFactor::InvFactor: return vk::BlendFactor::eOneMinusConstantColor;
	default:
		throw NotSupportException(NotSupportType::Enum);
	}
}

auto CodeRed::enumConvert(const CompareOperator op)
	-> vk::CompareOp
{
	switch (op) {
	case CompareOperator::Never: return vk::CompareOp::eNever;
	case CompareOperator::Less: return vk::CompareOp::eLess;
	case CompareOperator::Equal: return vk::CompareOp::eEqual;
	case CompareOperator::LessEqual: return vk::CompareOp::eLessOrEqual;
	case CompareOperator::Greater: return vk::CompareOp::eGreater;
	case CompareOperator::NotEqual: return vk::CompareOp::eNotEqual;
	case CompareOperator::GreaterEqual: return vk::CompareOp::eGreaterOrEqual;
	case CompareOperator::Always: return vk::CompareOp::eAlways;
	default:
		throw NotSupportException(NotSupportType::Enum);
	}
}

auto CodeRed::enumConvert(const StencilOperator op)
	-> vk::StencilOp
{
	switch (op) {
	case StencilOperator::Keep: return vk::StencilOp::eKeep;
	case StencilOperator::Zero: return vk::StencilOp::eZero;
	case StencilOperator::Replace: return vk::StencilOp::eReplace;
	case StencilOperator::IncrementAndClamp: return vk::StencilOp::eIncrementAndClamp;
	case StencilOperator::DecrementAndClamp: return vk::StencilOp::eDecrementAndClamp;
	case StencilOperator::Invert: return vk::StencilOp::eInvert;
	case StencilOperator::IncrementAndWrap: return vk::StencilOp::eIncrementAndWrap;
	case StencilOperator::DecrementAndWrap: return vk::StencilOp::eDecrementAndWrap;
	default:
		throw NotSupportException(NotSupportType::Enum);
	}
}

auto CodeRed::enumConvert(const PrimitiveTopology topology)
	-> vk::PrimitiveTopology
{
	switch (topology) {
	case PrimitiveTopology::TriangleList: return vk::PrimitiveTopology::eTriangleList;
	default:
		throw NotSupportException(NotSupportType::Enum);
	}
}

auto CodeRed::enumConvert(const FillMode mode)
	-> vk::PolygonMode
{
	switch (mode) {
	case FillMode::Wireframe: return vk::PolygonMode::eLine;
	case FillMode::Solid: return vk::PolygonMode::eFill;
	default:
		throw NotSupportException(NotSupportType::Enum);
	}
}

auto CodeRed::enumConvert(const CullMode mode)
	-> vk::CullModeFlags
{
	switch (mode) {
	case CullMode::None: return vk::CullModeFlagBits::eNone;
	case CullMode::Front: return vk::CullModeFlagBits::eFront;
	case CullMode::Back: return vk::CullModeFlagBits::eBack;
	default:
		throw NotSupportException(NotSupportType::Enum);
	}
}

auto CodeRed::enumConvert(const FrontFace face)
	-> vk::FrontFace
{
	switch (face) {
	case FrontFace::Clockwise: return vk::FrontFace::eClockwise;
	case FrontFace::CounterClockwise: return vk::FrontFace::eCounterClockwise;
	default:
		throw NotSupportException(NotSupportType::Enum);
	}
}

auto CodeRed::enumConvert(const ShaderType type)
	-> vk::ShaderStageFlagBits
{
	switch (type) {
	case ShaderType::Vertex: return vk::ShaderStageFlagBits::eVertex;
	case ShaderType::Pixel: return vk::ShaderStageFlagBits::eFragment;
	default:
		throw NotSupportException(NotSupportType::Enum);
	}
}

auto CodeRed::enumConvert(const AttachmentLoad load)
	-> vk::AttachmentLoadOp
{
	switch (load) {
	case AttachmentLoad::Clear: return vk::AttachmentLoadOp::eClear;
	case AttachmentLoad::Load: return vk::AttachmentLoadOp::eLoad;
	case AttachmentLoad::DontCare: return vk::AttachmentLoadOp::eDontCare;
	default:
		throw NotSupportException(NotSupportType::Enum);
	}
}

auto CodeRed::enumConvert(const AttachmentStore store)
	-> vk::AttachmentStoreOp
{
	switch (store) {
	case AttachmentStore::Store: return vk::AttachmentStoreOp::eStore;
	case AttachmentStore::DontCare: return vk::AttachmentStoreOp::eDontCare;
	default:
		throw NotSupportException(NotSupportType::Enum);
	}
}

auto CodeRed::enumConvert1(const Dimension dimension)
-> vk::ImageViewType
{
	switch (dimension) {
	case Dimension::Dimension1D: return vk::ImageViewType::e1D;
	case Dimension::Dimension2D: return vk::ImageViewType::e2D;
	case Dimension::Dimension3D: return vk::ImageViewType::e3D;
	default:
		throw NotSupportException(NotSupportType::Enum);
	}
}

auto CodeRed::enumConvert1(
	const ResourceLayout layout,
	const ResourceType type)
	-> vk::AccessFlags
{
	switch (layout) {
	case ResourceLayout::GeneralRead:
		switch (type) {
		case ResourceType::Buffer:
			return vk::AccessFlagBits::eUniformRead | vk::AccessFlagBits::eIndexRead | vk::AccessFlagBits::eVertexAttributeRead;
		case ResourceType::Texture:
			return vk::AccessFlagBits::eShaderRead;
		default:
			throw NotSupportException(NotSupportType::Enum);
		}
	case ResourceLayout::RenderTarget: return vk::AccessFlagBits::eColorAttachmentWrite;
	case ResourceLayout::DepthStencil:
		return vk::AccessFlagBits::eDepthStencilAttachmentRead | vk::AccessFlagBits::eDepthStencilAttachmentWrite;
	case ResourceLayout::CopyDestination: return vk::AccessFlagBits::eTransferWrite;
	case ResourceLayout::CopySource: return vk::AccessFlagBits::eTransferRead;
	case ResourceLayout::Present: return vk::AccessFlagBits::eColorAttachmentRead;
	default:
		throw NotSupportException(NotSupportType::Enum);
	}
}

#endif
