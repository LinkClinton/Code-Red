#include "../../Shared/Exception/FailedException.hpp"
#include "../../Shared/Exception/ZeroException.hpp"

#include "../DirectX12LogicalDevice.hpp"
#include "DirectX12Texture.hpp"

#ifdef __ENABLE__DIRECTX12__

CodeRed::DirectX12Texture::DirectX12Texture(
	const std::shared_ptr<GpuLogicalDevice>& device,
	const ResourceInfo& info) :
	GpuTexture(device, info)
{
	const auto property = std::get<TextureProperty>(mInfo.Property);
	
	D3D12_RESOURCE_DESC desc = {};

	desc.Dimension = enumConvert(property.Dimension);
	desc.Alignment = 0;
	desc.Width = property.Width;
	desc.Height = static_cast<UINT>(property.Height);
	desc.DepthOrArraySize = 1;
	desc.MipLevels = 0;
	desc.Format = enumConvert(property.PixelFormat);
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;
	desc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
	desc.Flags = enumConvert(mInfo.Usage);

	D3D12_HEAP_PROPERTIES heapProperties = {
		enumConvert(mInfo.Heap),
		D3D12_CPU_PAGE_PROPERTY_UNKNOWN,
		D3D12_MEMORY_POOL_UNKNOWN,
		1,1
	};

	D3D12_CLEAR_VALUE clearValue = {};

	clearValue.Format = desc.Format;
	clearValue.Color[0] = 0.0f;
	clearValue.Color[1] = 0.0f;
	clearValue.Color[2] = 0.0f;
	clearValue.Color[3] = 0.0f;
	
	const auto dxDevice = static_cast<DirectX12LogicalDevice*>(mDevice.get())->device();

	//get the real size in bytes of texture we will use
	std::get<TextureProperty>(mInfo.Property).Size =
		dxDevice->GetResourceAllocationInfo(0, 1, &desc).SizeInBytes;
	
	throwIfFailed(
		dxDevice->CreateCommittedResource(
			&heapProperties,
			D3D12_HEAP_FLAG_NONE,
			&desc,
			enumConvert(mInfo.Layout),
			&clearValue,
			IID_PPV_ARGS(&mTexture)),
		FailedException({ "ID3D12Resource of Texture" }, DebugType::Create));
}

CodeRed::DirectX12Texture::DirectX12Texture(
	const std::shared_ptr<GpuLogicalDevice>& device,
	const WRL::ComPtr<ID3D12Resource>& texture, 
	const ResourceInfo& info) :
	GpuTexture(device, info),
	mTexture(texture)
{
	const auto dxDevice = static_cast<DirectX12LogicalDevice*>(mDevice.get())->device();
	const auto desc = mTexture->GetDesc();

	//we only update size property
	//the other properties of ID3D12Resource we do not update
	//because it is not necessary to make info and desc same
	//we only need to make sure the important properties are same
	std::get<TextureProperty>(mInfo.Property).Size =
		dxDevice->GetResourceAllocationInfo(0, 1, &desc).SizeInBytes;
}

auto CodeRed::DirectX12Texture::mapMemory() const -> void* 
{
	void* data = nullptr;
	
	mTexture->Map(0, nullptr, &data);

	return data;
}

void CodeRed::DirectX12Texture::unmapMemory() const
{
	mTexture->Unmap(0, nullptr);
}

#endif
