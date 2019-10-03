#include "../Shared/Exception/InvalidException.hpp"
#include "../Shared/Exception/FailedException.hpp"

#include "DirectX12Resource/DirectX12Texture.hpp"
#include "DirectX12Resource/DirectX12Buffer.hpp"

#include "DirectX12DescriptorHeap.hpp"
#include "DirectX12ResourceLayout.hpp"
#include "DirectX12LogicalDevice.hpp"

#ifdef __ENABLE__DIRECTX12__

using namespace CodeRed::DirectX12;

CodeRed::DirectX12DescriptorHeap::DirectX12DescriptorHeap(
	const std::shared_ptr<GpuLogicalDevice>& device,
	const std::shared_ptr<GpuResourceLayout>& resource_layout) :
	GpuDescriptorHeap(device, resource_layout)
{
	const auto dxDevice = std::static_pointer_cast<DirectX12LogicalDevice>(mDevice)->device();

	D3D12_DESCRIPTOR_HEAP_DESC info;
	
	info.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	info.NodeMask = 0;
	info.NumDescriptors = mResourceLayout->mElements.empty() ? 1 : static_cast<UINT>(mResourceLayout->mElements.size());
	info.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	
	CODE_RED_THROW_IF_FAILED(
		dxDevice->CreateDescriptorHeap(
			&info,
			IID_PPV_ARGS(&mDescriptorHeap)),
		FailedException(DebugType::Create, { "ID3D12DescriptorHeap" })
	);

	mDescriptorSize = dxDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
}

void CodeRed::DirectX12DescriptorHeap::bindTexture(
	const std::shared_ptr<GpuTexture>& texture,
	const size_t index)
{
	CODE_RED_DEBUG_THROW_IF(
		index >= mResourceLayout->mElements.size(),
		InvalidException<size_t>({ "index" })
	);

	CODE_RED_DEBUG_THROW_IF(
		mResourceLayout->mElements[index].Type != ResourceType::Texture,
		InvalidException<ResourceType>({ "element(index).Type" })
	);

	const auto dxDevice = std::static_pointer_cast<DirectX12LogicalDevice>(mDevice)->device();
	const auto dxTexture = std::static_pointer_cast<DirectX12Texture>(texture);

	const D3D12_CPU_DESCRIPTOR_HANDLE cpuHandle = {
		mDescriptorHeap->GetCPUDescriptorHandleForHeapStart().ptr +
			static_cast<SIZE_T>(index)* mDescriptorSize
	};

	D3D12_SHADER_RESOURCE_VIEW_DESC view = {};

	view.Format = enumConvert(texture->format());
	view.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;

	switch (texture->dimension()) {
	case Dimension::Dimension1D:
	{
		view.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE1D;
		view.Texture1D.MostDetailedMip = 0;
		view.Texture1D.MipLevels = 1;
		view.Texture1D.ResourceMinLODClamp = 0.0f;
		break;
	}
	case Dimension::Dimension2D:
	{
		view.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
		view.Texture2D.MostDetailedMip = 0;
		view.Texture2D.MipLevels = 1;
		view.Texture2D.ResourceMinLODClamp = 0.0f;
		view.Texture2D.PlaneSlice = 0;
		break;
	}
	case Dimension::Dimension3D:
		view.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE3D;
		view.Texture3D.MostDetailedMip = 0;
		view.Texture3D.MipLevels = 1;
		view.Texture3D.ResourceMinLODClamp = 0.0f;
		break;
	}

	dxDevice->CreateShaderResourceView(dxTexture->texture().Get(), &view, cpuHandle);
}

void CodeRed::DirectX12DescriptorHeap::bindBuffer(
	const std::shared_ptr<GpuBuffer>& buffer,
	const size_t index)
{
	CODE_RED_DEBUG_THROW_IF(
		index >= mResourceLayout->mElements.size(),
		InvalidException<size_t>({ "index" })
	);

	CODE_RED_DEBUG_THROW_IF(
		mResourceLayout->mElements[index].Type != buffer->type() ||
		mResourceLayout->mElements[index].Type == ResourceType::Texture,
		InvalidException<ResourceType>({ "element(index).Type" })
	);

	const auto dxDevice = std::static_pointer_cast<DirectX12LogicalDevice>(mDevice)->device();
	const auto dxBuffer = std::static_pointer_cast<DirectX12Buffer>(buffer);

	const D3D12_CPU_DESCRIPTOR_HANDLE cpuHandle = {
		mDescriptorHeap->GetCPUDescriptorHandleForHeapStart().ptr +
			static_cast<SIZE_T>(index)* mDescriptorSize
	};

	
	switch (dxBuffer->type()) {
	case ResourceType::Buffer:
		{
			D3D12_CONSTANT_BUFFER_VIEW_DESC bufferView = {
				dxBuffer->buffer()->GetGPUVirtualAddress(),
				static_cast<UINT>(dxBuffer->size())
			};

			dxDevice->CreateConstantBufferView(&bufferView, cpuHandle);

			break;
		}
	case ResourceType::GroupBuffer:
		{
			D3D12_SHADER_RESOURCE_VIEW_DESC resourceView = {};

			resourceView.Format = DXGI_FORMAT_UNKNOWN;
			resourceView.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
			resourceView.ViewDimension = D3D12_SRV_DIMENSION_BUFFER;
			resourceView.Buffer.FirstElement = 0;
			resourceView.Buffer.NumElements = static_cast<UINT>(dxBuffer->count());
			resourceView.Buffer.Flags = D3D12_BUFFER_SRV_FLAG_NONE;
			resourceView.Buffer.StructureByteStride = static_cast<UINT>(dxBuffer->stride());

			dxDevice->CreateShaderResourceView(dxBuffer->buffer().Get(), &resourceView, cpuHandle);

			break;
		}
	case ResourceType::Texture:
	default:
		throw NotSupportException(NotSupportType::Enum);
	}
	
}
#endif
