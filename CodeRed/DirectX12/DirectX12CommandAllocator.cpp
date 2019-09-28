#include "../Shared/Exception/FailedException.hpp"

#include "DirectX12CommandAllocator.hpp"
#include "DirectX12LogicalDevice.hpp"

#ifdef __ENABLE__DIRECTX12__

using namespace CodeRed::DirectX12;

CodeRed::DirectX12CommandAllocator::DirectX12CommandAllocator(
	const std::shared_ptr<GpuLogicalDevice> device) :
	GpuCommandAllocator(device)
{
	const auto dxDevice = static_cast<DirectX12LogicalDevice*>(mDevice.get())->device();

	CODE_RED_THROW_IF_FAILED(
		dxDevice->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&mCommandAllocator)),
		FailedException({ "ID3D12CommandAllocator" }, DebugType::Create));
}

void CodeRed::DirectX12CommandAllocator::reset()
{
	mCommandAllocator->Reset();
}

#endif
