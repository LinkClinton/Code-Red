#include "../Shared/Exception/FailedException.hpp"

#include "DirectX12LogicalDevice.hpp"
#include "DirectX12CommandQueue.hpp"

#ifdef __ENABLE__DIRECTX12__

CodeRed::DirectX12CommandQueue::DirectX12CommandQueue(
	const std::shared_ptr<GpuLogicalDevice>& device) :
	GpuCommandQueue(device)
{
	const auto dxDevice = static_cast<DirectX12LogicalDevice*>(mDevice.get())->device();

	D3D12_COMMAND_QUEUE_DESC desc = {
		D3D12_COMMAND_LIST_TYPE_DIRECT,
		0,
		D3D12_COMMAND_QUEUE_FLAG_NONE,
		0
	};

	//create Direct3D 12 Command Queue
	throwIfFailed(
		dxDevice->CreateCommandQueue(&desc, IID_PPV_ARGS(&mCommandQueue)),
		FailedException({ "ID3D12CommandQueue" }, DebugType::Create));
}

#endif