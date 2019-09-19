#pragma once

#ifdef __CODE__RED__GLOBAL__INCLUDE__
#include <Interface/GpuPipelineState/GpuPipelineState.hpp>
#include <Shared/Enum/PrimitiveTopology.hpp>
#include <Shared/LayoutElement.hpp>
#else
#include "GpuPipelineState.hpp"
#include "../../Shared/Enum/PrimitiveTopology.hpp"
#include "../../Shared/LayoutElement.hpp"
#endif

#include <vector>

namespace CodeRed {

	class GpuInputAssemblyState : public GpuPipelineState {
	protected:
		GpuInputAssemblyState() = default;
		
		explicit GpuInputAssemblyState(
			const std::vector<InputLayoutElement>& elements,
			const PrimitiveTopology primitive_topology = PrimitiveTopology::Undefined) :
			mElements(elements), mPrimitiveTopology(primitive_topology) {}

		~GpuInputAssemblyState() = default;
	public:
		auto element(const size_t index) const -> const InputLayoutElement & { return mElements[index]; }
		
		auto elements() const noexcept -> std::vector<InputLayoutElement> { return mElements; }

		auto primitiveTopology() const noexcept -> PrimitiveTopology { return mPrimitiveTopology; }
	protected:
		std::vector<InputLayoutElement> mElements = {};
		
		PrimitiveTopology mPrimitiveTopology = PrimitiveTopology::Undefined;
	};
	
}