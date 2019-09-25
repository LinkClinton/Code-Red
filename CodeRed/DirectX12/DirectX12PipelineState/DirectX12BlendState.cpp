#include "DirectX12BlendState.hpp"

#ifdef __ENABLE__DIRECTX12__

CodeRed::DirectX12BlendState::DirectX12BlendState(
	const std::shared_ptr<GpuLogicalDevice> &device,
	const BlendProperty& blend_property) :
	GpuBlendState(device, blend_property)
{
	const auto& property = mBlendProperties[0];

	mBlendState.AlphaToCoverageEnable = false;
	mBlendState.IndependentBlendEnable = true;
	mBlendState.RenderTarget[0].BlendEnable = property.Enable;
	mBlendState.RenderTarget[0].LogicOpEnable = false;
	mBlendState.RenderTarget[0].SrcBlend = enumConvert(property.Source);
	mBlendState.RenderTarget[0].DestBlend = enumConvert(property.Destination);
	mBlendState.RenderTarget[0].BlendOp = enumConvert(property.ColorOperator);
	mBlendState.RenderTarget[0].SrcBlendAlpha = enumConvert(property.SourceAlpha);
	mBlendState.RenderTarget[0].DestBlendAlpha = enumConvert(property.DestinationAlpha);
	mBlendState.RenderTarget[0].BlendOpAlpha = enumConvert(property.AlphaOperator);
	mBlendState.RenderTarget[0].LogicOp = D3D12_LOGIC_OP_NOOP;
	mBlendState.RenderTarget[0].RenderTargetWriteMask = enumConvert(property.ColorMask);
}

#endif