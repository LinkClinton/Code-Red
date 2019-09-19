#pragma once

#ifdef __CODE__RED__GLOBAL__INCLUDE__
#include <Interface/GpuPipelineState/GpuPipelineState.hpp>
#include <Shared/BlendProperty.hpp>
#else
#include "GpuPipelineState.hpp"
#include "../../Shared/BlendProperty.hpp"
#endif

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