#pragma once

#include "Enum/ResourceType.hpp"
#include "Enum/ShaderVisable.hpp"
#include "Enum/PixelFormat.hpp"
#include "Utility.hpp"

#include <string>

namespace CodeRed {

	struct ResourceLayoutElement {
		ShaderVisable Visable;
		ResourceType Type;
		UInt32 Binding;
		UInt32 Space;
	};

	struct InputLayoutElement {
		PixelFormat Format;
		std::string Name;
	};
}
