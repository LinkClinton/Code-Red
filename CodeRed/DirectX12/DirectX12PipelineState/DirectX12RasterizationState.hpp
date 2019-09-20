#pragma once

#ifdef __CODE__RED__GLOBAL__INCLUDE__
#include <Interface/GpuPipelineState/GpuRasterizationState.hpp>
#include <DirectX12/DirectX12Utility.hpp>
#else
#include "../../Interface/GpuPipelineState/GpuRasterizationState.hpp"
#include "../DirectX12Utility.hpp"
#endif

#ifdef __ENABLE__DIRECTX12__

namespace CodeRed {

	class DirectX12RasterizationState final : public GpuRasterizationState {
	public:
		explicit DirectX12RasterizationState(
			const PixelFormat format,
			const FrontFace front_face = FrontFace::Clockwise,
			const CullMode cull_mode = CullMode::Back,
			const FillMode fill_mode = FillMode::Solid,
			const bool depth_clamp = true);

		~DirectX12RasterizationState() = default;

		auto constexpr state() const noexcept -> D3D12_RASTERIZER_DESC { return mRasterizationState; }
	private:
		D3D12_RASTERIZER_DESC mRasterizationState = {};
	};
	
}

#endif