#include "../Shared/Exception/FailedException.hpp"

#include "DirectX12GraphicsCommandList.hpp"
#include "DirectX12CommandAllocator.hpp"
#include "DirectX12LogicalDevice.hpp"

#ifdef __ENABLE__DIRECTX12__

CodeRed::DirectX12GraphicsCommandList::DirectX12GraphicsCommandList(
	const std::shared_ptr<GpuLogicalDevice>& device,
	const std::shared_ptr<GpuCommandAllocator>& allocator) :
	GpuGraphicsCommandList(device, allocator)
{
	const auto dxDevice = static_cast<DirectX12LogicalDevice*>(mDevice.get())->device();
	const auto dxAllocator = static_cast<DirectX12CommandAllocator*>(mAllocator.get())->allocator();

	throwIfFailed(
		dxDevice->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT,
			dxAllocator.Get(), nullptr, IID_PPV_ARGS(&mGraphicsCommandList)),
	FailedException({ "ID3D12GraphicsCommandList" }, DebugType::Create));
}

#endif
