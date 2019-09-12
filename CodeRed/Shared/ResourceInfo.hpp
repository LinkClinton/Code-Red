#pragma once

#include "Enum/MemoryHeap.hpp"
#include "Enum/ResourceUsage.hpp"
#include "Enum/PixelFormat.hpp"
#include "Enum/Dimension.hpp"

#include "PixelFormatSizeOf.hpp"

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
			const CodeRed::PixelFormat pixel_format,
			const CodeRed::Dimension dimension) :
			Width(width),
			Height(height),
			Depth(depth),
			PixelFormat(pixel_format),
			Dimension(dimension),
			Size(width * height * depth * PixelFormatSizeOf::get(pixel_format)) {}
	};

	using ResourceProperty =  std::variant<BufferProperty, TextureProperty>;
	
	struct ResourceInfo {
		ResourceProperty Property = BufferProperty();
		ResourceUsage Usage = ResourceUsage::None;
		MemoryHeap Heap = MemoryHeap::Default;
		
		ResourceInfo() = default;
		
		ResourceInfo(
			const ResourceProperty &resource_property,
			const ResourceUsage &resource_usage,
			const MemoryHeap &memory_heap) :
			Property(resource_property),
			Usage(resource_usage),
			Heap(memory_heap) {}

		static auto VertexBuffer(
			const size_t stride,
			const size_t count,
			const MemoryHeap heap) -> ResourceInfo
		{
			return ResourceInfo(
				BufferProperty(stride, count),
				ResourceUsage::VertexBuffer,
				heap);
		}

		static auto IndexBuffer(
			const size_t stride,
			const size_t count,
			const MemoryHeap heap) -> ResourceInfo
		{
			return ResourceInfo(
				BufferProperty(stride, count),
				ResourceUsage::IndexBuffer,
				heap);
		}

		static auto ConstantBuffer(
			const size_t size,
			const MemoryHeap heap) -> ResourceInfo
		{
			return ResourceInfo(
				BufferProperty(size, 1),
				ResourceUsage::ConstantBuffer,
				heap);
		}

		static auto Texture1D(
			const size_t width, 
			const PixelFormat pixel_format,
			const ResourceUsage resource_usage = ResourceUsage::None,
			const MemoryHeap heap = MemoryHeap::Default)
		{
			return ResourceInfo(
				TextureProperty(width, 1, 1, pixel_format, Dimension::Dimension1D),
				resource_usage,
				heap);
		}

		static auto Texture2D(
			const size_t width,
			const size_t height,
			const PixelFormat pixel_format,
			const ResourceUsage resource_usage = ResourceUsage::None,
			const MemoryHeap heap = MemoryHeap::Default)
		{
			return ResourceInfo(
				TextureProperty(width, height, 1, pixel_format, Dimension::Dimension1D),
				resource_usage,
				heap);
		}

		static auto Texture3D(
			const size_t width,
			const size_t height,
			const size_t depth,
			const PixelFormat pixel_format,
			const ResourceUsage resource_usage = ResourceUsage::None,
			const MemoryHeap heap = MemoryHeap::Default)
		{
			return ResourceInfo(
				TextureProperty(width, height, depth, pixel_format, Dimension::Dimension1D),
				resource_usage,
				heap);
		}
	};

}