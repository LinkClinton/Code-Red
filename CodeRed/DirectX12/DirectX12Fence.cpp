#include "../Shared/Exception/FailedException.hpp"

#include "DirectX12LogicalDevice.hpp"
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

#endif