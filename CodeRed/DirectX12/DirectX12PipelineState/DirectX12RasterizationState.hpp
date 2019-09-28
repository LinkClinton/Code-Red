#pragma once

#include "../../Interface/GpuPipelineState/GpuRasterizationState.hpp"
#include "../DirectX12Utility.hpp"

#ifdef __ENABLE__DIRECTX12__

namespace CodeRed {

	class DirectX12RasterizationState final : public GpuRasterizationState {
	public:
		explicit DirectX12RasterizationState(
			const std::shared_ptr<GpuLogicalDevice>& device,
			const FrontFace front_face = FrontFace::Clockwise,
			const CullMode cull_mode = CullMode::Back,
			const FillMode fill_mode = FillMode::Solid,
			const bool depth_clamp = true);

		~DirectX12RasterizationState() = default;

		auto state() const noexcept -> D3D12_RASTERIZER_DESC { return mRasterizationState; }
	private:
		D3D12_RASTERIZER_DESC mRasterizationState = {};
	};
	
}

#endif