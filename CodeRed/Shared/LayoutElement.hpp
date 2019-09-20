#pragma once

#ifdef __CODE__RED__GLOBAL__INCLUDE__
#include <Shared/Enum/ShaderVisibility.hpp>
#include <Shared/Enum/ResourceType.hpp>
#include <Shared/Enum/PixelFormat.hpp>
#include <Shared/Utility.hpp>
#else
#include "Enum/ShaderVisibility.hpp"
#include "Enum/ResourceType.hpp"
#include "Enum/PixelFormat.hpp"
#include "Utility.hpp"
#endif


#include <string>
#include <memory>

namespace CodeRed {

	class GpuSampler;
	
	struct ResourceLayoutElement {
		ShaderVisibility Visibility = ShaderVisibility::All;
		ResourceType Type = ResourceType::Buffer;
		UInt32 Binding = 0;
		UInt32 Space = 0;

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
		UInt32 Binding = 0;
		UInt32 Space = 0;

		std::shared_ptr<GpuSampler> Sampler;

		SamplerLayoutElement() = default;
		
		explicit SamplerLayoutElement(
			const std::shared_ptr<GpuSampler> &sampler,
			const UInt32 binding = 0,
			const UInt32 space = 0,
			const ShaderVisibility visibility = ShaderVisibility::All
		) : Visibility(visibility), Binding(binding), Space(space), Sampler(sampler) {}
	};

	struct InputLayoutElement {
		PixelFormat Format = PixelFormat::Unknown;
		std::string Name = "";

		InputLayoutElement() = default;
		
		explicit InputLayoutElement(
			const std::string &name,
			const PixelFormat format
		) : Format(format), Name(name) {}
	};
}
