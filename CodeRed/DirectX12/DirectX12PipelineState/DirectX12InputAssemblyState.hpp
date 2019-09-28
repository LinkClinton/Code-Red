#pragma once

#include "../../Interface/GpuPipelineState/GpuInputAssemblyState.hpp"
#include "../DirectX12Utility.hpp"

#ifdef __ENABLE__DIRECTX12__

namespace CodeRed {

	class DirectX12InputAssemblyState final : public GpuInputAssemblyState {
	public:
		explicit DirectX12InputAssemblyState(
			const std::shared_ptr<GpuLogicalDevice> &device,
			const std::vector<InputLayoutElement>& elements,
			const PrimitiveTopology primitive_topology = PrimitiveTopology::TriangleList);

		~DirectX12InputAssemblyState() = default;

		auto layout() const noexcept -> D3D12_INPUT_LAYOUT_DESC { return mInputLayout; }
	private:
		std::vector<D3D12_INPUT_ELEMENT_DESC> mInputLayoutElements = {};
		
		D3D12_INPUT_LAYOUT_DESC mInputLayout = { nullptr, 0 };
	};
	
}

#endif