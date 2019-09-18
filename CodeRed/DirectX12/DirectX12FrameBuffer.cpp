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
		static_cast<UINT>(render_target->size()),
		D3D12_DESCRIPTOR_HEAP_FLAG_NONE,
		0
	};

	D3D12_DESCRIPTOR_HEAP_DESC dsvHeapInfo = {
		D3D12_DESCRIPTOR_HEAP_TYPE_DSV,
		1,
		D3D12_DESCRIPTOR_HEAP_FLAG_NONE,
		0
	};

	throwIfFailed(
		dxDevice->CreateDescriptorHeap(&rtvHeapInfo, IID_PPV_ARGS(&mRenderTargetHeap)),
		FailedException({ "ID3D12DescriptorHeap of Render Target" }, DebugType::Create)
	);

	throwIfFailed(
		dxDevice->CreateDescriptorHeap(&dsvHeapInfo, IID_PPV_ARGS(&mDepthStencilHeap)),
		FailedException({ "ID3D12DescriptorHeap of Depth Stencil" }, DebugType::Create)
	);
	
	DirectX12FrameBuffer::reset(render_target, depth_stencil);
}

#endif