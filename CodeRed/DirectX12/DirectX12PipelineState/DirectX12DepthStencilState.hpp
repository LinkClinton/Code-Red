#pragma once

#include "../../Interface/GpuPipelineState/GpuDepthStencilState.hpp"
#include "../DirectX12Utility.hpp"

#ifdef __ENABLE__DIRECTX12__

namespace CodeRed {

	class DirectX12DepthStencilState final : public GpuDepthStencilState {
	public:
		explicit DirectX12DepthStencilState(
			const std::shared_ptr<GpuLogicalDevice> &device,
			const bool depth_enable = true,
			const bool depth_write_enable = true,
			const bool stencil_enable = false,
			const CompareOperator depth_operator = CompareOperator::LessEqual,
			const StencilOperatorInfo& front = StencilOperatorInfo(),
			const StencilOperatorInfo& back = StencilOperatorInfo());

		~DirectX12DepthStencilState() = default;

		auto state() const noexcept -> D3D12_DEPTH_STENCIL_DESC { return mDepthStencilDesc; }
	private:
		D3D12_DEPTH_STENCIL_DESC mDepthStencilDesc = {};
	};
	
}

#endif