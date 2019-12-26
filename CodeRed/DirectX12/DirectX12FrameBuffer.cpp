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
	
	//create rtv on render target heap
	CODE_RED_TRY_EXECUTE(
		mRenderTargets[0] != nullptr,
		dxDevice->CreateRenderTargetView(
			static_cast<DirectX12Texture*>(mRenderTargets[0].get())->texture().Get(),
			nullptr,
			mRenderTargetHeap->GetCPUDescriptorHandleForHeapStart()
		)
	);

	//create dsv on depth stencil heap
	CODE_RED_TRY_EXECUTE(
		mDepthStencil != nullptr,
		dxDevice->CreateDepthStencilView(
			static_cast<DirectX12Texture*>(mDepthStencil.get())->texture().Get(),
			nullptr,
			mDepthStencilHeap->GetCPUDescriptorHandleForHeapStart()
		)
	);
}

#endif
