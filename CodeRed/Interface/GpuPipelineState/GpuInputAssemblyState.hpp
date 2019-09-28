#pragma once

#include "GpuPipelineState.hpp"
#include "../../Shared/Enum/PrimitiveTopology.hpp"
#include "../../Shared/LayoutElement.hpp"

#include <vector>

namespace CodeRed {

	class GpuInputAssemblyState : public GpuPipelineState {
	protected:
		explicit GpuInputAssemblyState(
			const std::shared_ptr<GpuLogicalDevice> &device,
			const std::vector<InputLayoutElement>& elements,
			const PrimitiveTopology primitive_topology = PrimitiveTopology::TriangleList) :
			GpuPipelineState(device),
			mElements(elements), mPrimitiveTopology(primitive_topology) {}

		~GpuInputAssemblyState() = default;
	public:
		auto element(const size_t index) const -> const InputLayoutElement & { return mElements[index]; }
		
		auto elements() const noexcept -> std::vector<InputLayoutElement> { return mElements; }

		auto primitiveTopology() const noexcept -> PrimitiveTopology { return mPrimitiveTopology; }
	protected:
		std::vector<InputLayoutElement> mElements = {};
		
		PrimitiveTopology mPrimitiveTopology = PrimitiveTopology::TriangleList;
	};
	
}