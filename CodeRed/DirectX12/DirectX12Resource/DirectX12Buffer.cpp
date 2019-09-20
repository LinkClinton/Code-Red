#include "../../Shared/Exception/FailedException.hpp"
#include "../../Shared/Exception/ZeroException.hpp"
#include "../DirectX12LogicalDevice.hpp"

#include "DirectX12Buffer.hpp"


#ifdef __ENABLE__DIRECTX12__

CodeRed::DirectX12Buffer::DirectX12Buffer(
	const std::shared_ptr<GpuLogicalDevice>& device, 
	const ResourceInfo info) :
	GpuBuffer(device, info)
{
	D3D12_RESOURCE_DESC desc = {};

	desc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	desc.Alignment = 0;
	desc.Width = GpuBuffer::size();
	desc.Height = 1;
	desc.DepthOrArraySize = 1;
	desc.MipLevels = 1;
	desc.Format = DXGI_FORMAT_UNKNOWN;
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;
	desc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
	desc.Flags = enumConvert(mInfo.Usage);

	D3D12_HEAP_PROPERTIES heapProperties = {
		enumConvert(mInfo.Heap),
		D3D12_CPU_PAGE_PROPERTY_UNKNOWN,
		D3D12_MEMORY_POOL_UNKNOWN,
		1, 1
	};

	auto dxDevice = static_cast<DirectX12LogicalDevice*>(mDevice.get())->device();

	throwIfFailed(
		dxDevice->CreateCommittedResource(
			&heapProperties,
			D3D12_HEAP_FLAG_NONE,
			&desc,
			enumConvert(mInfo.Layout),
			nullptr,
			IID_PPV_ARGS(&mBuffer)
		),
		FailedException({ "ID3D12Resource of Buffer" }, DebugType::Create));
}

auto CodeRed::DirectX12Buffer::mapMemory() const -> void* 
{
	void* data = nullptr;

	mBuffer->Map(0, nullptr, &data);

	return data;
}

void CodeRed::DirectX12Buffer::unmapMemory() const
{
	mBuffer->Unmap(0, nullptr);
}

#endif
