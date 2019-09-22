#pragma once

#ifdef __CODE__RED__GLOBAL__INCLUDE__
#include <Shared/Enum/BlendOperator.hpp>
#include <Shared/Enum/BlendFactor.hpp>
#include <Shared/Enum/ColorMask.hpp>
#else
#include "Enum/BlendOperator.hpp"
#include "Enum/BlendFactor.hpp"
#include "Enum/ColorMask.hpp"
#endif

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

		BlendProperty(
			const bool enable,
			const BlendOperator colorOp = BlendOperator::Add,
			const BlendOperator alphaOp = BlendOperator::Add,
			const BlendFactor alphaDest = BlendFactor::Zero,
			const BlendFactor colorDest = BlendFactor::Zero,
			const BlendFactor alphaSrc = BlendFactor::One,
			const BlendFactor colorSrc = BlendFactor::One,
			const CodeRed::ColorMask mask = CodeRed::ColorMask::All
		) :
		ColorOperator(colorOp),
		AlphaOperator(alphaOp),
		DestinationAlpha(alphaDest),
		Destination(colorDest),
		SourceAlpha(alphaSrc),
		Source(colorSrc),
		ColorMask(mask),
		Enable(enable) {}
	};
	
}