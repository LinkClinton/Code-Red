#pragma once

#include "../Enum/ResourceLayout.hpp"
#include "../Enum/ResourceUsage.hpp"
#include "../Enum/ResourceType.hpp"
#include "../Enum/PixelFormat.hpp"
#include "../Enum/MultiSample.hpp"
#include "../Enum/MemoryHeap.hpp"
#include "../Enum/Dimension.hpp"

#include "../ClearValue.hpp"
#include "../PixelFormatSizeOf.hpp"
#include "../MultiSampleSizeOf.hpp"

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

		size_t MipLevels = 1;

		PixelFormat Format = PixelFormat::Unknown;
		MultiSample Sample = MultiSample::Count1;
		Dimension Dimension = Dimension::Dimension1D;
		
		
		ClearValue ClearValue = CodeRed::ClearValue();
		
		TextureProperty() = default;

		TextureProperty(
			const size_t width,
			const size_t height,
			const size_t depth,
			const size_t mipLevels,
			const CodeRed::PixelFormat format,
			const CodeRed::Dimension dimension,
			const CodeRed::MultiSample sample = MultiSample::Count1,
			const CodeRed::ClearValue& clearValue = CodeRed::ClearValue()) :
			Width(width),
			Height(height),
			Depth(depth),
			Size(width * height * (dimension == Dimension::Dimension3D ? depth : 1) * PixelFormatSizeOf::get(format) * MultiSampleSizeOf::get(sample)),
			MipLevels(mipLevels),
			Format(format),
			Sample(sample),
			Dimension(dimension),
			ClearValue(clearValue) {}
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

		static auto GroupBuffer(
			const size_t stride,
			const size_t count,
			const MemoryHeap heap = MemoryHeap::Upload,
			const ResourceLayout layout = ResourceLayout::GeneralRead) -> ResourceInfo
		{
			return ResourceInfo(
				BufferProperty(stride, count),
				layout,
				ResourceUsage::ConstantBuffer,
				ResourceType::GroupBuffer,
				heap
			);
		}
		
		static auto Texture1D(
			const size_t width, 
			const PixelFormat format,
			const size_t mipLevels = 1,
			const ResourceUsage usage = ResourceUsage::None)
		{
			return ResourceInfo(
				TextureProperty(width, 1, 1, mipLevels, format, Dimension::Dimension1D),
				ResourceLayout::GeneralRead, usage, 
				ResourceType::Texture, 
				MemoryHeap::Default);
		}

		static auto Texture1DArray(
			const size_t width,
			const size_t length,
			const PixelFormat format,
			const size_t mipLevels = 1,
			const ResourceUsage usage = ResourceUsage::None)
		{
			return ResourceInfo(
				TextureProperty(width, 1, length, mipLevels, format, Dimension::Dimension1D),
				ResourceLayout::GeneralRead, usage,
				ResourceType::Texture,
				MemoryHeap::Default);
		}
		
		static auto Texture2D(
			const size_t width,
			const size_t height,
			const PixelFormat format,
			const size_t mipLevels = 1,
			const ResourceUsage usage = ResourceUsage::None)
		{
			return Texture2DMultiSample(width, height, format,
				MultiSample::Count1, mipLevels, usage);
		}

		static auto Texture2DMultiSample(
			const size_t width,
			const size_t height,
			const PixelFormat format,
			const MultiSample sample,
			const size_t mipLevels = 1,
			const ResourceUsage usage = ResourceUsage::None) -> ResourceInfo
		{
			return ResourceInfo(
				TextureProperty(width, height, 1, mipLevels, format, Dimension::Dimension2D, sample),
				ResourceLayout::GeneralRead, usage,
				ResourceType::Texture,
				MemoryHeap::Default);
		}

		static auto CubeMap(
			const size_t width,
			const size_t height,
			const PixelFormat format,
			const size_t mipLevels = 1,
			const ResourceUsage usage = ResourceUsage::None)
		{
			return ResourceInfo(
				TextureProperty(width, height, 6, mipLevels, format, Dimension::Dimension2D),
				ResourceLayout::GeneralRead, usage,
				ResourceType::Texture,
				MemoryHeap::Default);
		}
		
		static auto Texture2DArray(
			const size_t width,
			const size_t height,
			const size_t length,
			const PixelFormat format,
			const size_t mipLevels = 1,
			const ResourceUsage usage = ResourceUsage::None)
		{
			return Texture2DMultiSampleArray(width, height, length, format,
				MultiSample::Count1, mipLevels, usage);
		}

		static auto Texture2DMultiSampleArray(
			const size_t width,
			const size_t height,
			const size_t length,
			const PixelFormat format,
			const MultiSample sample,
			const size_t mipLevels = 1,
			const ResourceUsage usage = ResourceUsage::None) -> ResourceInfo
		{
			return ResourceInfo(
				TextureProperty(width, height, length, mipLevels, format, Dimension::Dimension2D, sample),
				ResourceLayout::GeneralRead, usage,
				ResourceType::Texture,
				MemoryHeap::Default);
		}

		static auto CubeMapArray(
			const size_t width,
			const size_t height,
			const size_t length,
			const PixelFormat format,
			const size_t mipLevels = 1,
			const ResourceUsage usage = ResourceUsage::None)
		{
			return ResourceInfo(
				TextureProperty(width, height, 6 * length, mipLevels, format, Dimension::Dimension2D),
				ResourceLayout::GeneralRead, usage,
				ResourceType::Texture,
				MemoryHeap::Default);
		}

		static auto Texture3D(
			const size_t width,
			const size_t height,
			const size_t depth,
			const PixelFormat format,
			const size_t mipLevels = 1,
			const ResourceUsage usage = ResourceUsage::None)
		{
			return ResourceInfo(
				TextureProperty(width, height, depth, mipLevels, format, Dimension::Dimension3D),
				ResourceLayout::GeneralRead, usage,
				ResourceType::Texture,
				MemoryHeap::Default);
		}

		static auto RenderTarget(
			const size_t width,
			const size_t height,
			const PixelFormat format,
			const ClearValue& clearValue = ClearValue())
		{
			return RenderTargetMultiSample(width, height, format,
				MultiSample::Count1, clearValue
			);
		}

		static auto RenderTargetMultiSample(
			const size_t width,
			const size_t height,
			const PixelFormat format,
			const MultiSample sample,
			const ClearValue& clearValue = ClearValue()) -> ResourceInfo
		{
			return ResourceInfo(
				TextureProperty(width, height, 1, 1, format, Dimension::Dimension2D, sample, clearValue),
				ResourceLayout::GeneralRead,
				ResourceUsage::RenderTarget,
				ResourceType::Texture,
				MemoryHeap::Default);
		}
		
		static auto DepthStencil(
			const size_t width,
			const size_t height,
			const PixelFormat format,
			const ClearValue& clearValue = ClearValue())
		{
			return DepthStencilMultiSample(width, height, format,
				MultiSample::Count1, clearValue
			);
		}

		static auto DepthStencilMultiSample(
			const size_t width,
			const size_t height,
			const PixelFormat format,
			const MultiSample sample,
			const ClearValue& clearValue = ClearValue()) -> ResourceInfo
		{
			return ResourceInfo(
				TextureProperty(width, height, 1, 1, format, Dimension::Dimension2D, sample, clearValue),
				ResourceLayout::GeneralRead,
				ResourceUsage::DepthStencil,
				ResourceType::Texture,
				MemoryHeap::Default);
		}
	};

}
