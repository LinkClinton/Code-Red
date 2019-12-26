#include "../Shared/Exception/FailedException.hpp"
#include "../Shared/Exception/ZeroException.hpp"

#include "DirectX12Resource/DirectX12Texture.hpp"
#include "DirectX12LogicalDevice.hpp"
#include "DirectX12FrameBuffer.hpp"
#include "DirectX12TextureRef.hpp"

#ifdef __ENABLE__DIRECTX12__

using namespace CodeRed::DirectX12;

CodeRed::DirectX12FrameBuffer::DirectX12FrameBuffer(
	const std::shared_ptr<GpuLogicalDevice>& device,
	const std::shared_ptr<GpuTexture>& render_target, 
	const std::shared_ptr<GpuTexture>& depth_stencil) :
	DirectX12FrameBuffer(device, 
		render_target->reference(), 
		depth_stencil->reference())
{
	
}

CodeRed::DirectX12FrameBuffer::DirectX12FrameBuffer(
	const std::shared_ptr<GpuLogicalDevice>& device,
	const std::shared_ptr<GpuTextureRef>& render_target, 
	const std::shared_ptr<GpuTextureRef>& depth_stencil) :
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
	const std::shared_ptr<GpuTextureRef>& render_target,
	const std::shared_ptr<GpuTextureRef>& depth_stencil)
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

	if (mRenderTargets[0] != nullptr) {

		CODE_RED_DEBUG_THROW_IF(
			mRenderTargets[0]->mipLevel().size() != 1 || mRenderTargets[0]->array().size() != 1,
			"The size of mip level and array should be one, when we create a frame buffer."
		);
		
		D3D12_RENDER_TARGET_VIEW_DESC view;

		view.Format = enumConvert(mRenderTargets[0]->source()->format());

		if (mRenderTargets[0]->source()->isArray()) {

			view.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2DARRAY;
			view.Texture2DArray.FirstArraySlice = static_cast<UINT>(mRenderTargets[0]->array().Start);
			view.Texture2DArray.MipSlice = static_cast<UINT>(mRenderTargets[0]->mipLevel().Start);
			view.Texture2DArray.PlaneSlice = 0;
			view.Texture2DArray.ArraySize = 1;
			
		}else {

			view.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;
			view.Texture2D.MipSlice = static_cast<UINT>(mRenderTargets[0]->mipLevel().Start);
			view.Texture2D.PlaneSlice = 0;
			
		}

		dxDevice->CreateRenderTargetView(
			std::static_pointer_cast<DirectX12Texture>(mRenderTargets[0]->source())->texture().Get(),
			&view,
			mRenderTargetHeap->GetCPUDescriptorHandleForHeapStart()
		);
	}

	if (mDepthStencil != nullptr) {
		
		CODE_RED_DEBUG_THROW_IF(
			mDepthStencil->mipLevel().size() != 1 || mDepthStencil->array().size() != 1,
			"The size of mip level and array should be one, when we create a frame buffer."
		);

		D3D12_DEPTH_STENCIL_VIEW_DESC view;

		view.Format = enumConvert(mDepthStencil->source()->format());
		view.Flags = D3D12_DSV_FLAG_NONE;

		if (mDepthStencil->source()->isArray()) {

			view.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2DARRAY;
			view.Texture2DArray.FirstArraySlice = static_cast<UINT>(mDepthStencil->array().Start);
			view.Texture2DArray.MipSlice = static_cast<UINT>(mDepthStencil->mipLevel().Start);
			view.Texture2DArray.ArraySize = 1;

		}else {

			view.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
			view.Texture2D.MipSlice = static_cast<UINT>(mDepthStencil->mipLevel().Start);
			
		}

		dxDevice->CreateDepthStencilView(
			std::static_pointer_cast<DirectX12Texture>(mDepthStencil->source())->texture().Get(),
			&view,
			mDepthStencilHeap->GetCPUDescriptorHandleForHeapStart());
	}
}

#endif
