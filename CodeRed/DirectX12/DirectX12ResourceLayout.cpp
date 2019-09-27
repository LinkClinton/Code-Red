#include "../Shared/Exception/FailedException.hpp"

#include "DirectX12Resource/DirectX12Texture.hpp"
#include "DirectX12Resource/DirectX12Sampler.hpp"
#include "DirectX12Resource/DirectX12Buffer.hpp"
#include "DirectX12ResourceLayout.hpp"
#include "DirectX12LogicalDevice.hpp"

#include <algorithm>

#undef max

#ifdef __ENABLE__DIRECTX12__

CodeRed::DirectX12ResourceLayout::DirectX12ResourceLayout(
	const std::shared_ptr<GpuLogicalDevice> &device,
	const std::vector<ResourceLayoutElement>& elements,
	const std::vector<SamplerLayoutElement>& samplers,
	const size_t maxBindResources)
	: GpuResourceLayout(device, elements, samplers, maxBindResources)
{
	std::vector<D3D12_ROOT_PARAMETER> parameterArrays;
	std::vector<D3D12_STATIC_SAMPLER_DESC> samplerArrays;
	std::vector<D3D12_DESCRIPTOR_RANGE> ranges(elements.size());

	size_t index = 0;
	
	for (auto &element : mElements) {
		D3D12_ROOT_PARAMETER parameter = {};

		ranges[index] = {
			enumConvert(element.Type),
			1,
			static_cast<UINT>(element.Binding),
			static_cast<UINT>(element.Space),
			0
			};
		
		parameter.ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
		parameter.ShaderVisibility = enumConvert(element.Visibility);
		parameter.DescriptorTable.NumDescriptorRanges = 1;
		parameter.DescriptorTable.pDescriptorRanges = &ranges[index++];
		
		parameterArrays.push_back(parameter);
	}

	for (auto &sampler : mSamplers) {
		auto desc = static_cast<DirectX12Sampler*>(sampler.Sampler.get())->sampler();

		desc.ShaderVisibility = enumConvert(sampler.Visibility);
		desc.ShaderRegister = static_cast<UINT>(sampler.Binding);
		desc.RegisterSpace = static_cast<UINT>(sampler.Space);
		
		samplerArrays.push_back(desc);
	}

	D3D12_ROOT_SIGNATURE_DESC desc = {};

	desc.NumStaticSamplers = static_cast<UINT>(samplerArrays.size());
	desc.NumParameters = static_cast<UINT>(parameterArrays.size());
	desc.pStaticSamplers = samplerArrays.data();
	desc.pParameters = parameterArrays.data();
	desc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
	
	WRL::ComPtr<ID3DBlob> rootBlob;
	WRL::ComPtr<ID3DBlob> errorBlob;

	CODE_RED_THROW_IF_FAILED(
		D3D12SerializeRootSignature(&desc, D3D_ROOT_SIGNATURE_VERSION_1,
			rootBlob.GetAddressOf(), errorBlob.GetAddressOf()),
		FailedException({ "SerializeRootSignature" }, DebugType::Create));

	const auto dxDevice = std::static_pointer_cast<DirectX12LogicalDevice>(mDevice)->device();

	CODE_RED_THROW_IF_FAILED(
		dxDevice->CreateRootSignature(0, 
			rootBlob->GetBufferPointer(), 
			rootBlob->GetBufferSize(),
			IID_PPV_ARGS(&mRootSignature)),
		FailedException({ "ID3D12RootSignature" }, DebugType::Create));

	D3D12_DESCRIPTOR_HEAP_DESC heapInfo = {
		D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV,
		static_cast<UINT>(mMaxBindResources),
		D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE,
		0
	};

	CODE_RED_THROW_IF_FAILED(
		dxDevice->CreateDescriptorHeap(&heapInfo, IID_PPV_ARGS(&mDescriptorHeap)),
		FailedException({ "ID3D12DescriptorHeap" }, DebugType::Create)
	);

	mDescriptorSize = dxDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
}

void CodeRed::DirectX12ResourceLayout::reset()
{
	mIdentityAllocator.reset();
}

void CodeRed::DirectX12ResourceLayout::bindTexture(
	const size_t index,
	const std::shared_ptr<GpuTexture>& resource)
{
	auto& descriptors = mIdentityAllocator.container();

	//the resource is bound, so we do not need to bind again
	if (descriptors.find(resource) != descriptors.end()) return;

	CODE_RED_DEBUG_THROW_IF(
		descriptors.size() >= mMaxBindResources,
		Exception(
			"Bind resource to the resource layout failed."
			"Because the number of resources are too many.")
	);

	CODE_RED_DEBUG_THROW_IF(
		index >= mElements.size(),
		InvalidException<size_t>({ "index" })
	);

	CODE_RED_DEBUG_THROW_IF(
		mElements[index].Type != ResourceType::Texture,
		InvalidException<ResourceType>({ "element(index).Type" })
	);
	
	const auto dxDevice = std::static_pointer_cast<DirectX12LogicalDevice>(mDevice)->device();
	const auto texture = std::static_pointer_cast<DirectX12Texture>(resource);

	const auto identity = mIdentityAllocator.allocate();
	
	const D3D12_CPU_DESCRIPTOR_HANDLE cpuHandle = {
		mDescriptorHeap->GetCPUDescriptorHandleForHeapStart().ptr + 
			static_cast<SIZE_T>(identity) * mDescriptorSize
	};
	const D3D12_GPU_DESCRIPTOR_HANDLE gpuHandle = {
		mDescriptorHeap->GetGPUDescriptorHandleForHeapStart().ptr +
			static_cast<SIZE_T>(identity)* mDescriptorSize
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

	dxDevice->CreateShaderResourceView(texture->texture().Get(), &view, cpuHandle);
	
	descriptors.insert({ resource, gpuHandle });
}

void CodeRed::DirectX12ResourceLayout::bindBuffer(
	const size_t index,
	const std::shared_ptr<GpuBuffer>& resource)
{
	auto& descriptors = mIdentityAllocator.container();

	//the resource is bound, so we do not need to bind again
	if (descriptors.find(resource) != descriptors.end()) return;

	CODE_RED_DEBUG_THROW_IF(
		descriptors.size() >= mMaxBindResources,
		Exception(
			"Bind resource to the resource layout failed."
			"Because the number of resources are too many.")
	);

	CODE_RED_DEBUG_THROW_IF(
		index >= mElements.size(),
		InvalidException<size_t>({ "index" })
	);

	CODE_RED_DEBUG_THROW_IF(
		mElements[index].Type != ResourceType::Buffer,
		InvalidException<ResourceType>({ "element(index).Type" })
	);
	
	const auto dxDevice = std::static_pointer_cast<DirectX12LogicalDevice>(mDevice)->device();
	const auto buffer = std::static_pointer_cast<DirectX12Buffer>(resource);

	const auto identity = mIdentityAllocator.allocate();
	
	const D3D12_CPU_DESCRIPTOR_HANDLE cpuHandle = {
		mDescriptorHeap->GetCPUDescriptorHandleForHeapStart().ptr +
			static_cast<SIZE_T>(identity) * mDescriptorSize
	};
	const D3D12_GPU_DESCRIPTOR_HANDLE gpuHandle = {
		mDescriptorHeap->GetGPUDescriptorHandleForHeapStart().ptr +
			static_cast<SIZE_T>(identity)* mDescriptorSize
	};
	
	D3D12_CONSTANT_BUFFER_VIEW_DESC view = {
		buffer->buffer()->GetGPUVirtualAddress(),
		static_cast<UINT>(buffer->size())
	};

	dxDevice->CreateConstantBufferView(&view, cpuHandle);

	descriptors.insert({ resource, gpuHandle });
}

void CodeRed::DirectX12ResourceLayout::unbindResource(const std::shared_ptr<GpuResource>& resource)
{
	auto& descriptors = mIdentityAllocator.container();
	const auto it = descriptors.find(resource);
	
	CODE_RED_DEBUG_THROW_IF(
		it == descriptors.end(),
		Exception(
			"Unbind resource from resource layout failed."
			"Because the resource is not bound to the resource layout.")
	);

	if (it == descriptors.end()) return;

	mIdentityAllocator.free(
		(it->second.ptr - mDescriptorHeap->GetGPUDescriptorHandleForHeapStart().ptr) 
		/ mDescriptorSize);
	
	descriptors.erase(it);
}

auto CodeRed::DirectX12ResourceLayout::handle(
	const std::shared_ptr<GpuResource>& resource) 
	-> D3D12_GPU_DESCRIPTOR_HANDLE
{
	auto& container = mIdentityAllocator.container();

	CODE_RED_DEBUG_THROW_IF(
		container.find(resource) == container.end(),
		FailedException({ "Handle", "ResourceLayout" }, DebugType::Get)
	);
	
	return container[resource];
}

#endif
