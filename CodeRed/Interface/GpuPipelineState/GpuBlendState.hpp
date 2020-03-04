#pragma once

#include "GpuPipelineState.hpp"
#include "../../Shared/BlendProperty.hpp"

#include <vector>

namespace CodeRed {

	class GpuBlendState : public GpuPipelineState {
	protected:
		explicit GpuBlendState(
			const std::shared_ptr<GpuLogicalDevice>& device,
			const std::vector<BlendProperty>& blend_properties) :
			GpuPipelineState(device),
			mBlendProperties(blend_properties) {}
		
		~GpuBlendState() = default;
	public:
		auto property(const size_t index = 0) const -> const BlendProperty & { return mBlendProperties[index]; }
	protected:
		std::vector<BlendProperty> mBlendProperties = { BlendProperty() };
	};
	
}