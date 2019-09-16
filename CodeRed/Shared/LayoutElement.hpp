#pragma once

#include "Enum/ResourceType.hpp"
#include "Enum/ShaderVisable.hpp"
#include "Enum/PixelFormat.hpp"
#include "Utility.hpp"

#include <optional>
#include <memory>
#include <string>

namespace CodeRed {

	class GpuSampler;
	
	struct ResourceLayoutElement {
		ShaderVisable Visable = ShaderVisable::All;
		ResourceType Type = ResourceType::Buffer;
		UInt32 Binding = 0;
		UInt32 Space = 0;

		std::optional<std::shared_ptr<GpuSampler>> Sampler = std::nullopt;
	};

	struct InputLayoutElement {
		PixelFormat Format = PixelFormat::Unknown;
		std::string Name = "";
	};
}
