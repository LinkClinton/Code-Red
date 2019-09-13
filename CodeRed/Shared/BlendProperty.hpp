#pragma once

#include "Enum/BlendOperator.hpp"
#include "Enum/BlendFactor.hpp"
#include "Enum/ColorMask.hpp"

namespace CodeRed {

	struct BlendProperty {
		BlendOperator ColorOperator = BlendOperator::Add;
		BlendOperator AlphaOperator = BlendOperator::Add;
		
		BlendFactor DestinationAlpha = BlendFactor::Zero;
		BlendFactor Destination = BlendFactor::Zero;
		BlendFactor SourceAlpha = BlendFactor::One;
		BlendFactor Source = BlendFactor::One;

		ColorMask ColorMask = ColorMask::All;

		bool Enable = false;

		BlendProperty() = default;
	};
	
}