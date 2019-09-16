#include "../Shared/Exception/FailedException.hpp"

#include "DirectX12DisplayAdapter.hpp"
#include "DirectX12LogicalDevice.hpp"


#ifdef __ENABLE__DIRECTX12__

CodeRed::DirectX12LogicalDevice::DirectX12LogicalDevice(const std::shared_ptr<GpuDisplayAdapter>& adapter)
	: GpuLogicalDevice(adapter)
{
#ifdef _DEBUG
	//get the debug layer and enable debug
	WRL::ComPtr<ID3D12Debug3> debugLayer;
	D3D12GetDebugInterface(IID_PPV_ARGS(&debugLayer));
	debugLayer->EnableDebugLayer();
#endif

	//get the adapter we used to create device
	const auto dxgiAdapter = static_cast<DirectX12DisplayAdapter*>(adapter.get());

	//create Direct3D 12 Device.
	throwIfFailed(
		D3D12CreateDevice(dxgiAdapter->adapter().Get(), D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&mDevice)),
		FailedException({ "ID3D12Device" }, DebugType::Create));

	//create Direct3D 12 Command Allocator
	//Direct3D 12 Command Allocator is used for Graphics Command List
	throwIfFailed(
		mDevice->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&mCommandAllocator)),
		FailedException({ "ID3D12CommandAllocator" }, DebugType::Create));
}

#endif

