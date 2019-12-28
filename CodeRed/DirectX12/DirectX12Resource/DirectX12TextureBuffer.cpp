#include "DirectX12TextureBuffer.hpp"

#include "../../Shared/Exception/FailedException.hpp"
#include "../../Shared/Exception/ZeroException.hpp"

#include "../DirectX12LogicalDevice.hpp"
#include "DirectX12Texture.hpp"

#ifdef __ENABLE__DIRECTX12__

using namespace CodeRed::DirectX12;

CodeRed::DirectX12TextureBuffer::DirectX12TextureBuffer(
	const std::shared_ptr<GpuLogicalDevice>& device,
	const TextureBufferInfo& info) : GpuTextureBuffer(device, info)
{
	D3D12_RESOURCE_DESC desc = {};

	desc.Dimension = enumConvert(mInfo.Dimension);
	desc.Alignment = 0;
	desc.Width = mInfo.Width;
	desc.Height = static_cast<UINT>(mInfo.Height);
	desc.DepthOrArraySize = static_cast<UINT16>(mInfo.Depth);
	desc.MipLevels = 1;
	desc.Format = enumConvert(mInfo.Format);
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

	CODE_RED_THROW_IF_FAILED(
		dxDevice->CreateCommittedResource(
			&heapProperties,
			D3D12_HEAP_FLAG_NONE,
			&desc,
			enumConvert(mInfo.Layout),
			nullptr,
			IID_PPV_ARGS(&mTexture)),
		FailedException(DebugType::Create, { "ID3D12Resource of Texture" }));

	const auto allocateInfo = dxDevice->GetResourceAllocationInfo(0, 1, &desc);

	//because the size of texture is not always same in different adapters
	//so we need record the real size and the alignment
	mPhysicalSize = static_cast<size_t>(allocateInfo.SizeInBytes);
	mAlignment = static_cast<size_t>(allocateInfo.Alignment);
}

CodeRed::DirectX12TextureBuffer::DirectX12TextureBuffer(
	const std::shared_ptr<GpuLogicalDevice>& device,
	const std::shared_ptr<GpuTexture>& texture, 
	const size_t mipSlice) : DirectX12TextureBuffer(device, 
		TextureBufferInfo(
			texture->width(mipSlice),
			texture->height(mipSlice),
			texture->depth(mipSlice),
			texture->format(),
			texture->dimension()))
{
}

auto CodeRed::DirectX12TextureBuffer::read(const Extent3D<size_t>& extent) const -> std::vector<Byte>
{
	const auto rowPitch = mInfo.Width * PixelFormatSizeOf::get(mInfo.Format);
	const auto depthPitch = rowPitch * mInfo.Height;
	const auto region = convert(extent);
	
	std::vector<Byte> data(mInfo.Size);
	
	mTexture->ReadFromSubresource(data.data(),
		static_cast<UINT>(rowPitch),
		static_cast<UINT>(depthPitch),
		0, &region);

	return data;
}

auto CodeRed::DirectX12TextureBuffer::read() const -> std::vector<Byte>
{
	return read({ 0,0,0, width(), height(), depth() });
}

void CodeRed::DirectX12TextureBuffer::write(const Extent3D<size_t>& extent, const void* data)
{
	const auto rowPitch = mInfo.Width * PixelFormatSizeOf::get(mInfo.Format);
	const auto depthPitch = rowPitch * mInfo.Height;
	const auto region = convert(extent);
	
	mTexture->WriteToSubresource(0, &region, data,
		static_cast<UINT>(rowPitch),
		static_cast<UINT>(depthPitch));
}

void CodeRed::DirectX12TextureBuffer::write(const void* data)
{
	write({ 0,0,0, width(), height(), depth() }, data);
}

#endif
