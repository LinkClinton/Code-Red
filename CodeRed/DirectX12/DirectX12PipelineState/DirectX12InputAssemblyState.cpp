#include "DirectX12InputAssemblyState.hpp"

#include "../../Shared/PixelFormatSizeOf.hpp"

#ifdef __ENABLE__DIRECTX12__

CodeRed::DirectX12InputAssemblyState::DirectX12InputAssemblyState(
	const std::shared_ptr<GpuLogicalDevice> &device,
	const std::vector<InputLayoutElement>& elements,
	const PrimitiveTopology primitive_topology) :
	GpuInputAssemblyState(device, elements, primitive_topology)
{
	UINT offsetInBytes = 0;
	
	for (auto &element : mElements) {
		mInputLayoutElements.push_back({
			element.Name.c_str(),
			0,
			enumConvert(element.Format),
			0,
			offsetInBytes,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,
			0
			});

		offsetInBytes = offsetInBytes + static_cast<UINT>(PixelFormatSizeOf::get(element.Format));
	}

	mInputLayout.NumElements = static_cast<UINT>(mInputLayoutElements.size());
	mInputLayout.pInputElementDescs = mInputLayoutElements.data();
}

#endif