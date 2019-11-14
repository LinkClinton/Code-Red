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

	//because the D3D12_CLEAR_VALUE::Color and D3D12_CLEAR_VALUE::DepthStencil
	//is union struct, so we need set them carefully
	//if the texture is a rtv, we only can set clear value of color
	//if the texture is a dsv, we only can set clear value of depth
	if (enumHas(mInfo.Usage, ResourceUsage::RenderTarget)) {
		clearValue.Color[0] = property.ClearValue.Red;
		clearValue.Color[1] = property.ClearValue.Green;
		clearValue.Color[2] = property.ClearValue.Blue;
		clearValue.Color[3] = property.ClearValue.Alpha;
	}else {
		clearValue.DepthStencil.Depth = property.ClearValue.Depth;
		clearValue.DepthStencil.Stencil = property.ClearValue.Stencil;
	}
	
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

	const auto allocateInfo = dxDevice->GetResourceAllocationInfo(0, 1, &desc);

	//because the size of texture is not always same in different adapters
	//so we need record the real size and the alignment
	mPhysicalSize = static_cast<size_t>(allocateInfo.SizeInBytes);
	mAlignment = static_cast<size_t>(allocateInfo.Alignment);
}

CodeRed::DirectX12Texture::DirectX12Texture(
	const std::shared_ptr<GpuLogicalDevice>& device,
	const WRL::ComPtr<ID3D12Resource>& texture,
	const ResourceInfo& info) :
	GpuTexture(device, info),
	mTexture(texture)
{
	const auto dxDevice = static_cast<DirectX12LogicalDevice*>(mDevice.get())->device();
	const auto dxDesc = texture->GetDesc();
	const auto allocateInfo = dxDevice->GetResourceAllocationInfo(0, 1, &dxDesc);

	//because the size of texture is not always same in different adapters
	//so we need record the real size and the alignment
	mPhysicalSize = static_cast<size_t>(allocateInfo.SizeInBytes);
	mAlignment = static_cast<size_t>(allocateInfo.Alignment);
}

#endif
