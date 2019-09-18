#pragma once

#include <Interface/GpuPipelineState/GpuPipelineState.hpp>
#include <Shared/BlendProperty.hpp>

#include <vector>

namespace CodeRed {

	class GpuBlendState : public GpuPipelineState {
	protected:
		GpuBlendState() = default;
		
		explicit GpuBlendState(
			const BlendProperty& blend_property) :
			mBlendProperties({ blend_property }) {}
		
		~GpuBlendState() = default;
	public:
		auto property(const size_t index = 0) const -> const BlendProperty & { return mBlendProperties[index]; }
	protected:
		std::vector<BlendProperty> mBlendProperties = { BlendProperty() };
	};
	
}