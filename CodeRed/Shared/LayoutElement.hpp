#pragma once

#include "Enum/ShaderVisibility.hpp"
#include "Enum/ResourceType.hpp"
#include "Enum/PixelFormat.hpp"
#include "Utility.hpp"


#include <string>
#include <memory>

namespace CodeRed {

	class GpuSampler;
	
	struct ResourceLayoutElement {
		ShaderVisibility Visibility = ShaderVisibility::All;
		ResourceType Type = ResourceType::Buffer;
		size_t Binding = 0;
		size_t Space = 0;

		ResourceLayoutElement() = default;

		explicit ResourceLayoutElement(
			const ResourceType type,
			const UInt32 binding = 0,
			const UInt32 space = 0,
			const ShaderVisibility visibility = ShaderVisibility::All
		) : Visibility(visibility), Type(type), Binding(binding), Space(space) {}
	};

	struct SamplerLayoutElement {
		ShaderVisibility Visibility = ShaderVisibility::All;
		size_t Binding = 0;
		size_t Space = 0;

		std::shared_ptr<GpuSampler> Sampler;

		SamplerLayoutElement() = default;
		
		explicit SamplerLayoutElement(
			const std::shared_ptr<GpuSampler> &sampler,
			const size_t binding = 0,
			const size_t space = 0,
			const ShaderVisibility visibility = ShaderVisibility::All
		) : Visibility(visibility), Binding(binding), Space(space), Sampler(sampler) {}
	};

	struct InputLayoutElement {
		PixelFormat Format = PixelFormat::Unknown;
		std::string Name = "";
		size_t Slot = 0;

		InputLayoutElement() = default;
		
		explicit InputLayoutElement(
			const std::string &name,
			const PixelFormat format,
			const size_t slot = 0
		) : Format(format), Name(name), Slot(slot) {}
	};
}
