#include "../../Shared/Exception/FailedException.hpp"
#include "../../Shared/Exception/ZeroException.hpp"

#include "../DirectX12LogicalDevice.hpp"
#include "DirectX12Texture.hpp"

#ifdef __ENABLE__DIRECTX12__

using namespace CodeRed::DirectX12;

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
	desc.DepthOrArraySize = static_cast<UINT16>(property.Depth);
	desc.MipLevels = 1;
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
	clearValue.DepthStencil.Depth = 1.0f;
	clearValue.DepthStencil.Stencil = 0;

	const auto dxDevice = static_cast<DirectX12LogicalDevice*>(mDevice.get())->device();

	CODE_RED_THROW_IF_FAILED(
		dxDevice->CreateCommittedResource(
			&heapProperties,
			D3D12_HEAP_FLAG_NONE,
			&desc,
			enumConvert(mInfo.Layout),
			enumHas(mInfo.Usage, ResourceUsage::RenderTarget) || 
			enumHas(mInfo.Usage, ResourceUsage::DepthStencil) ? &clearValue : nullptr,
			IID_PPV_ARGS(&mTexture)),
		FailedException(DebugType::Create, { "ID3D12Resource of Texture" }));
}

CodeRed::DirectX12Texture::DirectX12Texture(
	const std::shared_ptr<GpuLogicalDevice>& device,
	const WRL::ComPtr<ID3D12Resource>& texture, 
	const ResourceInfo& info) :
	GpuTexture(device, info),
	mTexture(texture)
{
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
