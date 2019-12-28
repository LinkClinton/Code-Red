#include "../Shared/Exception/FailedException.hpp"

#include "DirectX12Resource/DirectX12Texture.hpp"
#include "DirectX12Resource/DirectX12Buffer.hpp"

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
		FailedException(DebugType::Create, { "ID3D12CommandAllocator" })
	);
}

void CodeRed::DirectX12CommandAllocator::reset()
{
	mCommandAllocator->Reset();

	//clear the cache resources that used for copy memory to buffer or texture
	//because the command list only record the commands, we need to keep the resource
	//until the queue finishes the copy commands
	//(we need keep the resource even the command list was destroyed)
	mCopyCacheResources = std::vector<std::shared_ptr<GpuResource>>();
}

auto CodeRed::DirectX12CommandAllocator::allocateCopyCacheBuffer(const size_t size)
	-> std::shared_ptr<GpuBuffer>
{
	const auto buffer = mDevice->createBuffer(
		ResourceInfo::UploadBuffer(size, 1)
	);

	mCopyCacheResources.push_back(buffer);

	return buffer;
}

auto CodeRed::DirectX12CommandAllocator::allocateCopyCacheTexture(const TextureProperty& property)
	-> std::shared_ptr<GpuTexture>
{
	D3D12_RESOURCE_DESC desc = {};

	desc.Dimension = enumConvert(property.Dimension);
	desc.Alignment = 0;
	desc.Width = property.Width;
	desc.Height = static_cast<UINT>(property.Height);
	desc.DepthOrArraySize = static_cast<UINT>(property.Depth);
	desc.MipLevels = 1;
	desc.Format = enumConvert(property.Format);
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;
	desc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
	desc.Flags = D3D12_RESOURCE_FLAG_NONE;

	D3D12_HEAP_PROPERTIES heapProperties = {
		D3D12_HEAP_TYPE_CUSTOM,
		D3D12_CPU_PAGE_PROPERTY_WRITE_BACK,
		D3D12_MEMORY_POOL_L0,
		1,1
	};

	const auto dxDevice = static_cast<DirectX12LogicalDevice*>(mDevice.get())->device();

	WRL::ComPtr<ID3D12Resource> dxTexture;
	
	CODE_RED_THROW_IF_FAILED(
		dxDevice->CreateCommittedResource(
			&heapProperties,
			D3D12_HEAP_FLAG_NONE,
			&desc,
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS(&dxTexture)),
		FailedException(DebugType::Create, { "ID3D12Resource of Texture" }));

	//create a texture using ID3D12Resource
	//this texture is only used to copy memory data to texture
	//we first copy memory to this texture with special heap
	//then we copy this texture to the texture with default heap
	const auto texture = std::make_shared<DirectX12Texture>(mDevice, dxTexture,
		ResourceInfo(property,
			ResourceLayout::GeneralRead,
			ResourceUsage::None,
			ResourceType::Texture,
			MemoryHeap::Upload));

	mCopyCacheResources.push_back(texture);

	return texture;
}

#endif
