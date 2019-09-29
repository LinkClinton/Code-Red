#pragma once

#include "../Enum/ResourceLayout.hpp"
#include "../Enum/ResourceUsage.hpp"
#include "../Enum/ResourceType.hpp"
#include "../Enum/PixelFormat.hpp"
#include "../Enum/MemoryHeap.hpp"
#include "../Enum/Dimension.hpp"

#include "../PixelFormatSizeOf.hpp"

#include <variant>

namespace CodeRed {

	struct BufferProperty {
		size_t Stride = 0;
		size_t Count = 0;
		size_t Size = 0;

		BufferProperty() = default;

		BufferProperty(
			const size_t stride, 
			const size_t count) :
			Stride(stride),
			Count(count),
			Size(stride * count) {}
	};
	
	struct TextureProperty {
		size_t Width = 0;
		size_t Height = 0;
		size_t Depth = 0;
		size_t Size = 0;

		PixelFormat PixelFormat = PixelFormat::Unknown;
		Dimension Dimension = Dimension::Dimension1D;

		TextureProperty() = default;

		TextureProperty(
			const size_t width,
			const size_t height,
			const size_t depth,
			const CodeRed::PixelFormat format,
			const CodeRed::Dimension dimension) :
			Width(width),
			Height(height),
			Depth(depth),
			Size(width * height * depth * PixelFormatSizeOf::get(format)),
			PixelFormat(format),
			Dimension(dimension) {}
	};

	using ResourceProperty =  std::variant<BufferProperty, TextureProperty>;
	
	struct ResourceInfo {
		ResourceProperty Property = BufferProperty();
		ResourceLayout Layout = ResourceLayout::GeneralRead;
		ResourceUsage Usage = ResourceUsage::None;
		ResourceType Type = ResourceType::Buffer;
		MemoryHeap Heap = MemoryHeap::Default;
		
		ResourceInfo() = default;
		
		ResourceInfo(
			const ResourceProperty& property,
			const ResourceLayout layout,
			const ResourceUsage usage,
			const ResourceType type,
			const MemoryHeap heap) :
			Property(property),
			Layout(layout),
			Usage(usage),
			Type(type),
			Heap(heap) {}

		static auto VertexBuffer(
			const size_t stride,
			const size_t count,
			const MemoryHeap heap = MemoryHeap::Default,
			const ResourceLayout layout = ResourceLayout::GeneralRead) -> ResourceInfo
		{
			return ResourceInfo(
				BufferProperty(stride, count),
				layout, ResourceUsage::VertexBuffer, ResourceType::Buffer, heap);
		}

		static auto IndexBuffer(
			const size_t stride,
			const size_t count,
			const MemoryHeap heap = MemoryHeap::Default,
			const ResourceLayout layout = ResourceLayout::GeneralRead) -> ResourceInfo
		{
			return ResourceInfo(
				BufferProperty(stride, count),
				layout, ResourceUsage::IndexBuffer, ResourceType::Buffer, heap);
		}

		static auto ConstantBuffer(
			const size_t size,
			const MemoryHeap heap = MemoryHeap::Upload,
			const ResourceLayout layout = ResourceLayout::GeneralRead) -> ResourceInfo
		{
			return ResourceInfo(
				BufferProperty(size, 1),
				layout, ResourceUsage::ConstantBuffer, ResourceType::Buffer, heap);
		}

		static auto UploadBuffer(
			const size_t stride,
			const size_t count) -> ResourceInfo
		{
			return ResourceInfo(
				BufferProperty(stride, count),
				ResourceLayout::GeneralRead,
				ResourceUsage::ConstantBuffer,
				ResourceType::Buffer,
				MemoryHeap::Upload
			);
		}

		static auto Texture1D(
			const size_t width, 
			const PixelFormat format,
			const ResourceUsage usage = ResourceUsage::None,
			const ResourceLayout layout = ResourceLayout::GeneralRead,
			const MemoryHeap heap = MemoryHeap::Default)
		{
			return ResourceInfo(
				TextureProperty(width, 1, 1, format, Dimension::Dimension1D),
				layout, usage, ResourceType::Texture, heap);
		}

		static auto Texture2D(
			const size_t width,
			const size_t height,
			const PixelFormat format,
			const ResourceUsage usage = ResourceUsage::None,
			const ResourceLayout layout = ResourceLayout::GeneralRead,
			const MemoryHeap heap = MemoryHeap::Default)
		{
			return ResourceInfo(
				TextureProperty(width, height, 1, format, Dimension::Dimension2D),
				layout, usage, ResourceType::Texture, heap);
		}

		static auto Texture3D(
			const size_t width,
			const size_t height,
			const size_t depth,
			const PixelFormat format,
			const ResourceUsage usage = ResourceUsage::None,
			const ResourceLayout layout = ResourceLayout::GeneralRead,
			const MemoryHeap heap = MemoryHeap::Default)
		{
			return ResourceInfo(
				TextureProperty(width, height, depth, format, Dimension::Dimension3D),
				layout, usage, ResourceType::Texture, heap);
		}
	};

}
