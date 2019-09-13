#pragma once

#include "../../Shared/Enum/PrimitiveTopology.hpp"
#include "../../Shared/LayoutElement.hpp"
#include "GpuPipelineState.hpp"

#include <vector>

namespace CodeRed {

	class GpuInputAssemblyState : public GpuPipelineState {
	public:
		explicit GpuInputAssemblyState(
			const std::vector<InputLayoutElement>& elements,
			const PrimitiveTopology primitive_topology = PrimitiveTopology::Undefined) :
			mInputLayoutElements(elements), mPrimitiveTopology(primitive_topology) {}

		~GpuInputAssemblyState() = default;

		auto element(const size_t index) const -> const InputLayoutElement & { return mInputLayoutElements[index]; }
		
		auto elements() const noexcept -> std::vector<InputLayoutElement> { return mInputLayoutElements; }

		auto primitiveTopology() const noexcept -> PrimitiveTopology { return mPrimitiveTopology; }
	protected:
		std::vector<InputLayoutElement> mInputLayoutElements;
		PrimitiveTopology mPrimitiveTopology;
	};
	
}