#pragma once

#include "Enum/ResourceType.hpp"
#include "Enum/ShaderVisable.hpp"
#include "Utility.hpp"

namespace CodeRed {

	struct ResourceLayoutElement {
		ShaderVisable Visable;
		ResourceType Type;
		UInt32 Binding;
		UInt32 Space;
	};
	
}