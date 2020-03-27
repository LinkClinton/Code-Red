#include "DirectX12RasterizationState.hpp"

#ifdef __ENABLE__DIRECTX12__

using namespace CodeRed::DirectX12;

CodeRed::DirectX12RasterizationState::DirectX12RasterizationState(
	const std::shared_ptr<GpuLogicalDevice>& device,
	const FrontFace front_face, 
	const CullMode cull_mode, 
	const FillMode fill_mode, 
	const bool depth_clamp) :
	GpuRasterizationState(
		device,
		front_face,
		cull_mode,
		fill_mode,
		depth_clamp
	)
{
	mRasterizationState.FillMode = enumConvert(mFillMode);
	mRasterizationState.CullMode = enumConvert(mCullMode);
	mRasterizationState.FrontCounterClockwise = enumConvert(mFrontFace);
	mRasterizationState.DepthBias = 0;
	mRasterizationState.DepthBiasClamp = 0.0f;
	mRasterizationState.SlopeScaledDepthBias = 0.0f;
	mRasterizationState.DepthClipEnable = mDepthClamp;
	mRasterizationState.MultisampleEnable = true;
	mRasterizationState.AntialiasedLineEnable = false;
	mRasterizationState.ForcedSampleCount = 0;
	mRasterizationState.ConservativeRaster = D3D12_CONSERVATIVE_RASTERIZATION_MODE_OFF;
}

#endif