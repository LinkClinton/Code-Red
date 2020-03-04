#include "DirectX12BlendState.hpp"

#ifdef __ENABLE__DIRECTX12__

using namespace CodeRed::DirectX12;

CodeRed::DirectX12BlendState::DirectX12BlendState(
	const std::shared_ptr<GpuLogicalDevice> &device,
	const std::vector<BlendProperty>& blend_properties) :
	GpuBlendState(device, blend_properties)
{
	mBlendState.AlphaToCoverageEnable = false;
	mBlendState.IndependentBlendEnable = true;

	for (size_t index = 0; index < mBlendProperties.size(); index++) {
		const auto& property = mBlendProperties[index];
		
		mBlendState.RenderTarget[index].BlendEnable = property.Enable;
		mBlendState.RenderTarget[index].LogicOpEnable = false;
		mBlendState.RenderTarget[index].SrcBlend = enumConvert(property.Source);
		mBlendState.RenderTarget[index].DestBlend = enumConvert(property.Destination);
		mBlendState.RenderTarget[index].BlendOp = enumConvert(property.ColorOperator);
		mBlendState.RenderTarget[index].SrcBlendAlpha = enumConvert(property.SourceAlpha);
		mBlendState.RenderTarget[index].DestBlendAlpha = enumConvert(property.DestinationAlpha);
		mBlendState.RenderTarget[index].BlendOpAlpha = enumConvert(property.AlphaOperator);
		mBlendState.RenderTarget[index].LogicOp = D3D12_LOGIC_OP_NOOP;
		mBlendState.RenderTarget[index].RenderTargetWriteMask = enumConvert(property.ColorMask);
	}
}

#endif