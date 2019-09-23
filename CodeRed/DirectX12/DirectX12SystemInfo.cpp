#include "../Shared/Exception/FailedException.hpp"
#include "../Shared/DebugReport.hpp"

#include "DirectX12DisplayAdapter.hpp"
#include "DirectX12SystemInfo.hpp"
#include "DirectX12Utility.hpp"

#ifdef __ENABLE__DIRECTX12__

auto CodeRed::DirectX12SystemInfo::selectDisplayAdapter() const -> std::vector<std::shared_ptr<GpuDisplayAdapter>>
{
	WRL::ComPtr<IDXGIFactory4> factory;
	WRL::ComPtr<IDXGIAdapter1> adapter;

	std::vector<std::shared_ptr<GpuDisplayAdapter>> display_adapters;
	
	CODE_RED_THROW_IF_FAILED(CreateDXGIFactory2(DXGI_CREATE_FACTORY_DEBUG, IID_PPV_ARGS(&factory)),
		FailedException( {"DXGIFactory"}, DebugType::Create));

	UINT index = 0;

	while (factory->EnumAdapters1(index++, &adapter) != DXGI_ERROR_NOT_FOUND) {
		DXGI_ADAPTER_DESC1 desc;

		//if we get properties failed, we skip to record this adapter
		if (adapter->GetDesc1(&desc) != S_OK) {
			DebugReport::warning(DebugType::Get, { "properties", "adapter" });

			continue;
		}
		
		display_adapters.push_back(
			std::make_shared<DirectX12DisplayAdapter>(
				adapter,
				wideStringToMultiString(desc.Description), desc.DeviceId, desc.VendorId));
	}

	return display_adapters;
}

#endif