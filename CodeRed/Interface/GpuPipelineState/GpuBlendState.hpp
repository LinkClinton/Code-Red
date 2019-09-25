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
		explicit GpuBlendState(
			const std::shared_ptr<GpuLogicalDevice> &device,
			const BlendProperty& blend_property) :
			GpuPipelineState(device),
			mBlendProperties({ blend_property }) {}
		
		~GpuBlendState() = default;
	public:
		auto property(const size_t index = 0) const -> const BlendProperty & { return mBlendProperties[index]; }
	protected:
		std::vector<BlendProperty> mBlendProperties = { BlendProperty() };
	};
	
}