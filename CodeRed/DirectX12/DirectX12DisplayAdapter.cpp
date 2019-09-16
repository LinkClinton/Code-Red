#include "DirectX12DisplayAdapter.hpp"

#ifdef __ENABLE__DIRECTX12__

CodeRed::DirectX12DisplayAdapter::DirectX12DisplayAdapter(
	const WRL::ComPtr<IDXGIAdapter1> &adapter,
	const std::string &name,
	const UInt32 device_id,
	const UInt32 vendor_id) :
	GpuDisplayAdapter(name, device_id, vendor_id),
	mAdapter(adapter)
{
	
}

#endif

