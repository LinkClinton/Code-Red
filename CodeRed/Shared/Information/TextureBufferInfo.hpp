#pragma once

#include "../Enum/ResourceLayout.hpp"
#include "../Enum/PixelFormat.hpp"
#include "../Enum/Dimension.hpp"

#include "../PixelFormatSizeOf.hpp"

namespace CodeRed {

	struct TextureBufferInfo {
		size_t Width = 0;
		size_t Height = 0;
		size_t Depth = 0;

		size_t Size = 0;
		
		PixelFormat Format = PixelFormat::Unknown;
		Dimension Dimension = Dimension::Dimension1D;
		ResourceLayout Layout = ResourceLayout::GeneralRead;

		TextureBufferInfo() = default;

		TextureBufferInfo(
			const size_t width,
			const size_t height,
			const size_t depth,
			const PixelFormat format,
			const CodeRed::Dimension dimension) :
			Width(width), Height(height), Depth(depth), Size(PixelFormatSizeOf::get(format) * width * height * depth),
			Format(format), Dimension(dimension) {}

		static auto Texture1D(const size_t width, const PixelFormat format) -> TextureBufferInfo
		{
			return TextureBufferInfo(width, 1, 1, format, Dimension::Dimension1D);
		}

		static auto Texture2D(const size_t width, const size_t height, PixelFormat format) -> TextureBufferInfo
		{
			return TextureBufferInfo(width, height, 1, format, Dimension::Dimension2D);
		}

		static auto Texture3D(const size_t width, const size_t height, const size_t depth, PixelFormat format) -> TextureBufferInfo
		{
			return TextureBufferInfo(width, height, depth, format, Dimension::Dimension3D);
		}
	};
	
}