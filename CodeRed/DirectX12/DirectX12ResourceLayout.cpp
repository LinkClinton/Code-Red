#include "../Shared/Exception/FailedException.hpp"

#include "DirectX12Resource/DirectX12Sampler.hpp"
#include "DirectX12ResourceLayout.hpp"
#include "DirectX12LogicalDevice.hpp"

#ifdef __ENABLE__DIRECTX12__

CodeRed::DirectX12ResourceLayout::DirectX12ResourceLayout(
	const std::shared_ptr<GpuLogicalDevice> &device,
	const std::vector<ResourceLayoutElement>& elements,
	const std::vector<SamplerLayoutElement>& samplers)
	: GpuResourceLayout(device, elements, samplers)
{
	std::vector<D3D12_ROOT_PARAMETER> parameterArrays;
	std::vector<D3D12_STATIC_SAMPLER_DESC> samplerArrays;

	for (auto &element : mElements) {
		D3D12_ROOT_PARAMETER parameter = {};

		parameter.ParameterType = enumConvert(element.Type);
		parameter.ShaderVisibility = enumConvert(element.Visibility);
		parameter.Descriptor.ShaderRegister = static_cast<UINT>(element.Binding);
		parameter.Descriptor.RegisterSpace = static_cast<UINT>(element.Space);

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

	throwIfFailed(
		D3D12SerializeRootSignature(&desc, D3D_ROOT_SIGNATURE_VERSION_1,
			rootBlob.GetAddressOf(), errorBlob.GetAddressOf()),
		FailedException({ "SerializeRootSignature" }, DebugType::Create));

	auto dxDevice = static_cast<DirectX12LogicalDevice*>(mDevice.get())->device();
	
	throwIfFailed(
		dxDevice->CreateRootSignature(0, 
			rootBlob->GetBufferPointer(), 
			rootBlob->GetBufferSize(),
			IID_PPV_ARGS(&mRootSignature)),
		FailedException({ "ID3D12RootSignature" }, DebugType::Create));
}

#endif