#pragma once

#include <Interface/GpuPipelineState/GpuPipelineState.hpp>
#include <Shared/Enum/PrimitiveTopology.hpp>
#include <Shared/LayoutElement.hpp>

#include <vector>

namespace CodeRed {

	class GpuInputAssemblyState : public GpuPipelineState {
	protected:
		GpuInputAssemblyState() = default;
		
		explicit GpuInputAssemblyState(
			const std::vector<InputLayoutElement>& elements,
			const PrimitiveTopology primitive_topology = PrimitiveTopology::Undefined) :
			mInputLayoutElements(elements), mPrimitiveTopology(primitive_topology) {}

		~GpuInputAssemblyState() = default;
	public:
		auto element(const size_t index) const -> const InputLayoutElement & { return mInputLayoutElements[index]; }
		
		auto elements() const noexcept -> std::vector<InputLayoutElement> { return mInputLayoutElements; }

		auto primitiveTopology() const noexcept -> PrimitiveTopology { return mPrimitiveTopology; }
	protected:
		std::vector<InputLayoutElement> mInputLayoutElements = {};
		
		PrimitiveTopology mPrimitiveTopology = PrimitiveTopology::Undefined;
	};
	
}