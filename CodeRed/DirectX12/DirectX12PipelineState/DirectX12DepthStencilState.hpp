#pragma once

#ifdef __CODE__RED__GLOBAL__INCLUDE__
#include <Interface/GpuPipelineState/GpuDepthStencilState.hpp>
#include <DirectX12/DirectX12Utility.hpp>
#else
#include "../../Interface/GpuPipelineState/GpuDepthStencilState.hpp"
#include "../DirectX12Utility.hpp"
#endif

#ifdef __ENABLE__DIRECTX12__

namespace CodeRed {

	class DirectX12DepthStencilState final : public GpuDepthStencilState {
	public:
		explicit DirectX12DepthStencilState(
			const PixelFormat format,
			const bool depth_enable = true,
			const bool depth_write_enable = true,
			const bool stencil_enable = false,
			const CompareOperator depth_operator = CompareOperator::LessEqual,
			const StencilOperatorInfo& front = StencilOperatorInfo(),
			const StencilOperatorInfo& back = StencilOperatorInfo());

		~DirectX12DepthStencilState() = default;

		auto constexpr state() const noexcept -> D3D12_DEPTH_STENCIL_DESC { return mDepthStencilDesc; }
	private:
		D3D12_DEPTH_STENCIL_DESC mDepthStencilDesc;
	};
	
}

#endif