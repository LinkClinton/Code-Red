#include "../Shared/Exception/FailedException.hpp"

#include "DirectX12LogicalDevice.hpp"
#include "DirectX12CommandQueue.hpp"
#include "DirectX12Fence.hpp"

#ifdef __ENABLE__DIRECTX12__

CodeRed::DirectX12Fence::DirectX12Fence(
	const std::shared_ptr<GpuLogicalDevice>& device) :
	GpuFence(device)
{
	const auto dxDevice = static_cast<DirectX12LogicalDevice*>(mDevice.get())->device();

	throwIfFailed(
		dxDevice->CreateFence(mFenceValue, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&mFence)),
		FailedException({ "ID3D12Fence" }, DebugType::Create));
}

void CodeRed::DirectX12Fence::wait(const WRL::ComPtr<ID3D12CommandQueue>& queue)
{
	queue->Signal(mFence.Get(), ++mFenceValue);;

	if (mFence->GetCompletedValue() < mFenceValue) {
		const auto event_handle = CreateEventEx(nullptr, nullptr, false, EVENT_ALL_ACCESS);

		mFence->SetEventOnCompletion(mFenceValue, event_handle);

		if (event_handle != nullptr) {
			WaitForSingleObject(event_handle, INFINITE);
			CloseHandle(event_handle);
		}
		else DebugReport::warning(DebugType::Create, { "wait event" });
	}
}

#endif
