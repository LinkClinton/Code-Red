#include "../Shared/Exception/FailedException.hpp"
#include "../Shared/Exception/ZeroException.hpp"

#include "DirectX12Resource/DirectX12Texture.hpp"
#include "DirectX12LogicalDevice.hpp"
#include "DirectX12FrameBuffer.hpp"

#ifdef __ENABLE__DIRECTX12__

using namespace CodeRed::DirectX12;

CodeRed::DirectX12FrameBuffer::DirectX12FrameBuffer(
	const std::shared_ptr<GpuLogicalDevice>& device,
	const std::shared_ptr<GpuTexture>& render_target, 
	const std::shared_ptr<GpuTexture>& depth_stencil) :
	GpuFrameBuffer(device, render_target, depth_stencil)
{
	const auto dxDevice = static_cast<DirectX12LogicalDevice*>(mDevice.get())->device();

	D3D12_DESCRIPTOR_HEAP_DESC rtvHeapInfo = {
		D3D12_DESCRIPTOR_HEAP_TYPE_RTV,
		1,
		D3D12_DESCRIPTOR_HEAP_FLAG_NONE,
		0
	};

	D3D12_DESCRIPTOR_HEAP_DESC dsvHeapInfo = {
		D3D12_DESCRIPTOR_HEAP_TYPE_DSV,
		1,
		D3D12_DESCRIPTOR_HEAP_FLAG_NONE,
		0
	};

	CODE_RED_THROW_IF_FAILED(
		dxDevice->CreateDescriptorHeap(&rtvHeapInfo, IID_PPV_ARGS(&mRenderTargetHeap)),
		FailedException(DebugType::Create, { "ID3D12DescriptorHeap of Render Target" })
	);

	CODE_RED_THROW_IF_FAILED(
		dxDevice->CreateDescriptorHeap(&dsvHeapInfo, IID_PPV_ARGS(&mDepthStencilHeap)),
		FailedException(DebugType::Create, { "ID3D12DescriptorHeap of Depth Stencil" })
	);
	
	DirectX12FrameBuffer::reset(render_target, depth_stencil);
}

void CodeRed::DirectX12FrameBuffer::reset(
	const std::shared_ptr<GpuTexture>& render_target,
	const std::shared_ptr<GpuTexture>& depth_stencil)
{
	//reset the render target and depth stencil
	for (auto& rtv : mRenderTargets) rtv.reset();

	mDepthStencil.reset();

	const auto dxDevice = static_cast<DirectX12LogicalDevice*>(mDevice.get())->device();
	
	mRenderTargets[0] = render_target;
	mDepthStencil = depth_stencil;
	
	//warning, when we create a frame buffer without rtv and dsv
	//only output when we enable __ENABLE__CODE__RED__DEBUG__
	CODE_RED_DEBUG_TRY_EXECUTE(
		mRenderTargets[0] == nullptr &&
		mDepthStencil == nullptr,
		DebugReport::warning(DebugType::Create, 
			{ "FrameBuffer" },
			{ "there are no rtv and dsv." })
	);

	CODE_RED_DEBUG_THROW_IF(
		mRenderTargets[0]->arrayRange().size() != 1 || mRenderTargets[0]->mipRange().size() != 1 ||
		mDepthStencil->arrayRange().size() != 1 || mDepthStencil->mipRange().size() != 1,
		"The range of array and mip level is not one, we will use the first texture as frame buffer."
	);
	
	D3D12_RENDER_TARGET_VIEW_DESC rtvView;

	rtvView.Format = enumConvert(mRenderTargets[0]->format());
	rtvView.ViewDimension = mRenderTargets[0]->isArray() ? D3D12_RTV_DIMENSION_TEXTURE2DARRAY : D3D12_RTV_DIMENSION_TEXTURE2D;

	//if the render target is texture array, we will use the first texture
	if (mRenderTargets[0]->isArray()) {
		rtvView.Texture2DArray.ArraySize = 1;
		rtvView.Texture2DArray.FirstArraySlice = static_cast<UINT>(mRenderTargets[0]->arrayRange().Start);
		rtvView.Texture2DArray.MipSlice = static_cast<UINT>(mRenderTargets[0]->mipRange().Start);
		rtvView.Texture2DArray.PlaneSlice = 0;
	}else {
		rtvView.Texture2D.MipSlice = static_cast<UINT>(mRenderTargets[0]->mipRange().Start);
		rtvView.Texture2D.PlaneSlice = 0;
	}
	
	//create rtv on render target heap
	CODE_RED_TRY_EXECUTE(
		mRenderTargets[0] != nullptr,
		dxDevice->CreateRenderTargetView(
			static_cast<DirectX12Texture*>(mRenderTargets[0].get())->texture().Get(),
			&rtvView,
			mRenderTargetHeap->GetCPUDescriptorHandleForHeapStart()
		)
	);

	
	D3D12_DEPTH_STENCIL_VIEW_DESC dsvView;

	dsvView.Format = enumConvert(mDepthStencil->format());
	dsvView.ViewDimension = mDepthStencil->isArray() ? D3D12_DSV_DIMENSION_TEXTURE2DARRAY : D3D12_DSV_DIMENSION_TEXTURE2D;
	dsvView.Flags = D3D12_DSV_FLAG_NONE;

	//if the depth stencil is texture array, we will use the first texture
	if (mDepthStencil->isArray()) {
		dsvView.Texture2DArray.ArraySize = 1;
		dsvView.Texture2DArray.FirstArraySlice = static_cast<UINT>(mDepthStencil->arrayRange().Start);
		dsvView.Texture2DArray.MipSlice = static_cast<UINT>(mDepthStencil->mipRange().Start);
	}else {
		dsvView.Texture2D.MipSlice = static_cast<UINT>(mDepthStencil->mipRange().Start);
	}
	
	//create dsv on depth stencil heap
	CODE_RED_TRY_EXECUTE(
		mDepthStencil != nullptr,
		dxDevice->CreateDepthStencilView(
			static_cast<DirectX12Texture*>(mDepthStencil.get())->texture().Get(),
			&dsvView,
			mDepthStencilHeap->GetCPUDescriptorHandleForHeapStart()
		)
	);
}

#endif
