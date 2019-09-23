#include "../Shared/Exception/FailedException.hpp"

#include "DirectX12GraphicsCommandList.hpp"
#include "DirectX12LogicalDevice.hpp"
#include "DirectX12CommandQueue.hpp"
#include "DirectX12Fence.hpp"

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
	CODE_RED_THROW_IF_FAILED(
		dxDevice->CreateCommandQueue(&desc, IID_PPV_ARGS(&mCommandQueue)),
		FailedException({ "ID3D12CommandQueue" }, DebugType::Create));
}

void CodeRed::DirectX12CommandQueue::execute(
	const std::vector<std::shared_ptr<GpuGraphicsCommandList>>& lists)
{
	std::vector<ID3D12CommandList*> dxLists;

	for (auto &list : lists) {
		dxLists.push_back(
			static_cast<DirectX12GraphicsCommandList*>(list.get())->commandList().Get()
		);
	}

	mCommandQueue->ExecuteCommandLists(
		static_cast<UINT>(dxLists.size()), dxLists.data());
}

void CodeRed::DirectX12CommandQueue::waitIdle()
{
	static auto fence = std::make_shared<DirectX12Fence>(mDevice);

	fence->wait(mCommandQueue);
}

#endif
