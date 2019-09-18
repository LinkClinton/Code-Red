#include "DirectX12Sampler.hpp"

#ifdef __ENABLE__DIRECTX12__

CodeRed::DirectX12Sampler::DirectX12Sampler(
	const std::shared_ptr<GpuLogicalDevice>& device,
	const SamplerInfo& info) :
	GpuSampler(device, info)
{
	mSampler.Filter = enumConvert(mInfo.Filter);
	mSampler.AddressU = enumConvert(mInfo.AddressModeU);
	mSampler.AddressV = enumConvert(mInfo.AddressModeV);
	mSampler.AddressW = enumConvert(mInfo.AddressModeW);
	mSampler.BorderColor = enumConvert(mInfo.Border);
	mSampler.MipLODBias = 0;
	mSampler.MaxAnisotropy = mInfo.MaxAnisotropy;
	mSampler.ComparisonFunc = D3D12_COMPARISON_FUNC_LESS_EQUAL;
	mSampler.MinLOD = 0.0f;
	mSampler.MaxLOD = D3D12_FLOAT32_MAX;
}

#endif