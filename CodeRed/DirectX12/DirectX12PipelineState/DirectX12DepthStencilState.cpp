#include "DirectX12DepthStencilState.hpp"

#ifdef __ENABLE__DIRECTX12__

CodeRed::DirectX12DepthStencilState::DirectX12DepthStencilState(
	const PixelFormat format,
	const bool depth_enable, 
	const bool depth_write_enable, 
	const bool stencil_enable,
	const CompareOperator depth_operator, 
	const StencilOperatorInfo& front, 
	const StencilOperatorInfo& back) :
	GpuDepthStencilState(
		format,
		depth_enable,
		depth_write_enable,
		stencil_enable,
		depth_operator,
		front,
		back
	)
{
	mDepthStencilDesc.DepthEnable = mDepthEnable;
	mDepthStencilDesc.DepthWriteMask = mDepthWriteEnable ? D3D12_DEPTH_WRITE_MASK_ALL : D3D12_DEPTH_WRITE_MASK_ZERO;
	mDepthStencilDesc.DepthFunc = enumConvert(mDepthOperator);
	mDepthStencilDesc.StencilEnable = mStencilEnable;
	mDepthStencilDesc.StencilReadMask = D3D12_DEFAULT_STENCIL_READ_MASK;
	mDepthStencilDesc.StencilWriteMask = D3D12_DEFAULT_STENCIL_READ_MASK;
	mDepthStencilDesc.FrontFace.StencilFunc = enumConvert(mFrontFace.CompareOperator);
	mDepthStencilDesc.BackFace.StencilFunc = enumConvert(mBackFace.CompareOperator);
	mDepthStencilDesc.FrontFace.StencilDepthFailOp = enumConvert(mFrontFace.DepthFailOperator);
	mDepthStencilDesc.BackFace.StencilDepthFailOp = enumConvert(mBackFace.DepthFailOperator);
	mDepthStencilDesc.FrontFace.StencilPassOp = enumConvert(mFrontFace.PassOperator);
	mDepthStencilDesc.BackFace.StencilPassOp = enumConvert(mBackFace.PassOperator);
	mDepthStencilDesc.FrontFace.StencilFailOp = enumConvert(mFrontFace.FailOperator);
	mDepthStencilDesc.BackFace.StencilFailOp = enumConvert(mBackFace.FailOperator);
}


#endif