#pragma once

#include "../../Shared/BlendProperty.hpp"
#include "GpuPipelineState.hpp"

#include <vector>

namespace CodeRed {

	class GpuBlendState : public GpuPipelineState {
	public:
		GpuBlendState() = default;
		
		explicit GpuBlendState(
			const BlendProperty& blend_property) :
			mBlendProperties({ blend_property }) {}
		
		~GpuBlendState() = default;

		auto property(const size_t index = 0) const -> const BlendProperty & { return mBlendProperties[index]; }
	protected:
		std::vector<BlendProperty> mBlendProperties = { BlendProperty() };
	};
	
}