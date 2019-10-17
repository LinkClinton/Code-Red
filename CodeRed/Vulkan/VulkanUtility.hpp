#pragma once

#ifdef __CODE__RED__GLOBAL__INCLUDE__
#include <Shared/Utility.hpp>
#else
#include "../Shared/Utility.hpp"
#endif

#ifdef __ENABLE__VULKAN__

#ifdef _WIN32
#define VK_USE_PLATFORM_WIN32_KHR
#endif

#include <vulkan/vulkan.hpp>

#define CODE_RED_THROW_IF(condition, exception) if (condition) throw exception;

namespace CodeRed {

	enum class PrimitiveTopology : UInt32;
	enum class ShaderVisibility : UInt32;
	enum class CompareOperator : UInt32;
	enum class StencilOperator : UInt32;
	enum class AttachmentStore : UInt32;
	enum class ResourceLayout : UInt32;
	enum class AttachmentLoad : UInt32;
	enum class FilterOptions : UInt32;
	enum class BlendOperator : UInt32;
	enum class ResourceUsage : UInt32;
	enum class ResourceType : UInt32;
	enum class PixelFormat : UInt32;
	enum class AddressMode : UInt32;
	enum class BlendFactor : UInt32;
	enum class BorderColor : UInt32;
	enum class ShaderType : UInt32;
	enum class MemoryHeap : UInt32;
	enum class FrontFace : UInt32;
	enum class IndexType : UInt32;
	enum class Dimension : UInt32;
	enum class ColorMask : UInt32;
	enum class FillMode : UInt32;
	enum class CullMode : UInt32;

	namespace Vulkan {

		using VulkanResourceUsage = std::pair<vk::BufferUsageFlags, vk::ImageUsageFlags>;

		auto enumConvert(const FilterOptions filter, const size_t index)->vk::Filter;

		auto enumConvert(const FilterOptions filter)->vk::SamplerMipmapMode;

		auto enumConvert(const AddressMode mode)->vk::SamplerAddressMode;

		auto enumConvert(const BorderColor color)->vk::BorderColor;

		auto enumConvert(const ResourceType type)->vk::DescriptorType;

		auto enumConvert(const ShaderVisibility visibility)->vk::ShaderStageFlagBits;

		auto enumConvert(const ResourceLayout layout)->vk::ImageLayout;

		auto enumConvert(const ResourceUsage usage)->VulkanResourceUsage;

		auto enumConvert(const MemoryHeap heap)->vk::MemoryPropertyFlags;

		auto enumConvert(const PixelFormat format)->vk::Format;

		auto enumConvert(const Dimension dimension)->vk::ImageType;

		auto enumConvert(const ColorMask mask)->vk::ColorComponentFlags;

		auto enumConvert(const BlendOperator op)->vk::BlendOp;

		auto enumConvert(const BlendFactor factor)->vk::BlendFactor;

		auto enumConvert(const CompareOperator op)->vk::CompareOp;

		auto enumConvert(const StencilOperator op)->vk::StencilOp;

		auto enumConvert(const PrimitiveTopology topology)->vk::PrimitiveTopology;

		auto enumConvert(const FillMode mode)->vk::PolygonMode;

		auto enumConvert(const CullMode mode)->vk::CullModeFlags;

		auto enumConvert(const FrontFace face)->vk::FrontFace;

		auto enumConvert(const IndexType type)->vk::IndexType;
		
		auto enumConvert(const ShaderType type)->vk::ShaderStageFlagBits;

		auto enumConvert(const AttachmentLoad load)->vk::AttachmentLoadOp;

		auto enumConvert(const AttachmentStore store)->vk::AttachmentStoreOp;

		auto enumConvert(const PixelFormat format, const ResourceUsage usage) -> vk::ImageAspectFlags;
		
		auto enumConvert1(const Dimension dimension)->vk::ImageViewType;

		auto enumConvert1(const ResourceLayout layout, const ResourceType type)->vk::AccessFlags;

	}
}

#endif