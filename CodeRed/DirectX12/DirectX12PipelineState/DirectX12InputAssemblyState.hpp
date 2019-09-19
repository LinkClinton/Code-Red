#pragma once

#ifdef __CODE__RED__GLOBAL__INCLUDE__
#include <Interface/GpuPipelineState/GpuInputAssemblyState.hpp>
#include <DirectX12/DirectX12Utility.hpp>
#else
#include "../../Interface/GpuPipelineState/GpuInputAssemblyState.hpp"
#include "../DirectX12Utility.hpp"
#endif

#ifdef __ENABLE__DIRECTX12__

namespace CodeRed {

	class DirectX12InputAssemblyState final : public GpuInputAssemblyState {
	public:
		DirectX12InputAssemblyState() = default;
		
		explicit DirectX12InputAssemblyState(
			const std::vector<InputLayoutElement>& elements,
			const PrimitiveTopology primitive_topology = PrimitiveTopology::Undefined);

		~DirectX12InputAssemblyState() = default;

		auto constexpr layout() const noexcept -> D3D12_INPUT_LAYOUT_DESC { return mInputLayout; }
	private:
		std::vector<D3D12_INPUT_ELEMENT_DESC> mInputLayoutElements = {};
		
		D3D12_INPUT_LAYOUT_DESC mInputLayout = { nullptr, 0 };
	};
	
}

#endif