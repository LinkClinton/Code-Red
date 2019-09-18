#pragma once

#include <Shared/Enum/BlendOperator.hpp>
#include <Shared/Enum/BlendFactor.hpp>
#include <Shared/Enum/ColorMask.hpp>

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