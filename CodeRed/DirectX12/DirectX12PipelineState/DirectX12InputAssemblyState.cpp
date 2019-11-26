#include "DirectX12InputAssemblyState.hpp"

#include "../../Shared/PixelFormatSizeOf.hpp"

#ifdef __ENABLE__DIRECTX12__

using namespace CodeRed::DirectX12;

CodeRed::DirectX12InputAssemblyState::DirectX12InputAssemblyState(
	const std::shared_ptr<GpuLogicalDevice> &device,
	const std::vector<InputLayoutElement>& elements,
	const PrimitiveTopology primitive_topology) :
	GpuInputAssemblyState(device, elements, primitive_topology)
{
	auto offsetInBytes = std::vector<UINT>(mSlotCount);
	
	for (auto &element : mElements) {
		mInputLayoutElements.push_back({
			element.Name.c_str(),
			0,
			enumConvert(element.Format),
			static_cast<UINT>(element.Slot),
			offsetInBytes[element.Slot],
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,
			0
			});

		offsetInBytes[element.Slot] = offsetInBytes[element.Slot] + static_cast<UINT>(PixelFormatSizeOf::get(element.Format));
	}

	mInputLayout.NumElements = static_cast<UINT>(mInputLayoutElements.size());
	mInputLayout.pInputElementDescs = mInputLayoutElements.data();
}

#endif