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
	//if we enable CODE_RED_DEBUG
	//we will throw the zero exception when we get zero value
	//current size is not the real size we will used
	CODE_RED_DEBUG_THROW_IF(
		std::get<BufferProperty>(mInfo.Property).Size == 0,
		ZeroException<size_t>({ "info.Property.Size" })
	);

	//the buffer can not be used to "render target" or "depth stencil"
	CODE_RED_DEBUG_THROW_IF(
		enumHas(mInfo.Usage, ResourceUsage::RenderTarget) ||
		enumHas(mInfo.Usage, ResourceUsage::DepthStencil),
		InvalidException<ResourceUsage>({ "info.Usage" })
	);

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
	desc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
	desc.Flags = enumConvert(mInfo.Usage);

	D3D12_HEAP_PROPERTIES heapProperties = {
		enumConvert(mInfo.Heap),
		D3D12_CPU_PAGE_PROPERTY_UNKNOWN,
		D3D12_MEMORY_POOL_UNKNOWN,
		1, 1
	};

	auto dxDevice = static_cast<DirectX12LogicalDevice*>(mDevice.get())->device();

	//get the real size in bytes of buffer we will use
	std::get<BufferProperty>(mInfo.Property).Size = 
		dxDevice->GetResourceAllocationInfo(0, 1, &desc).SizeInBytes;
	
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

#endif