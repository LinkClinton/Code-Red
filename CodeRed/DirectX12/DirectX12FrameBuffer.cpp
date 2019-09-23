#include "../Shared/Exception/FailedException.hpp"

#include "DirectX12Resource/DirectX12Texture.hpp"
#include "DirectX12LogicalDevice.hpp"
#include "DirectX12FrameBuffer.hpp"

#ifdef __ENABLE__DIRECTX12__

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
		FailedException({ "ID3D12DescriptorHeap of Render Target" }, DebugType::Create)
	);

	CODE_RED_THROW_IF_FAILED(
		dxDevice->CreateDescriptorHeap(&dsvHeapInfo, IID_PPV_ARGS(&mDepthStencilHeap)),
		FailedException({ "ID3D12DescriptorHeap of Depth Stencil" }, DebugType::Create)
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

	//create rtv on render target heap
	//if the render target is nullptr, we do not create it
	if (render_target != nullptr) {
		mRenderTargets[0] = render_target;
		
		dxDevice->CreateRenderTargetView(
			static_cast<DirectX12Texture*>(mRenderTargets[0].get())->texture().Get(),
			nullptr,
			mRenderTargetHeap->GetCPUDescriptorHandleForHeapStart()
		);
	}

	//create dsv on depth stencil heap
	//if the depth stencil is nullptr, we do not create it
	if (depth_stencil != nullptr) {
		mDepthStencil = depth_stencil;

		dxDevice->CreateDepthStencilView(
			static_cast<DirectX12Texture*>(mDepthStencil.get())->texture().Get(),
			nullptr,
			mDepthStencilHeap->GetCPUDescriptorHandleForHeapStart()
		);
	}
}

#endif
