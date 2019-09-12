#pragma once

#include "../../Shared/Enum/PrimitiveTopology.hpp"
#include "../../Shared/LayoutElement.hpp"
#include "GpuPipelineStage.hpp"

#include <vector>

namespace CodeRed {

	class GpuInputAssemblyStage : public GpuPipelineStage {
	public:
		GpuInputAssemblyStage(
			const std::vector<InputLayoutElement>& elements,
			const PrimitiveTopology primitive_topology = PrimitiveTopology::Undefined) :
			mInputLayoutElements(elements), mPrimitiveTopology(primitive_topology) {}

		auto elements() const -> std::vector<InputLayoutElement> { return mInputLayoutElements; }
		
		auto primitiveTopology() const -> PrimitiveTopology { return mPrimitiveTopology; }
	protected:
		std::vector<InputLayoutElement> mInputLayoutElements;
		PrimitiveTopology mPrimitiveTopology;
	};
	
}