#include "DirectX12DisplayAdapter.hpp"

#ifdef __ENABLE__DIRECTX12__

CodeRed::DirectX12DisplayAdapter::DirectX12DisplayAdapter(
	const WRL::ComPtr<IDXGIAdapter1> &adapter,
	const std::string &name,
	const size_t device_id,
	const size_t vendor_id) :
	GpuDisplayAdapter(name, device_id, vendor_id),
	mAdapter(adapter)
{
	
}

#endif

