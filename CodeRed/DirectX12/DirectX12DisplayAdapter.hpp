#pragma once

#ifdef __ENABLE__DIRECTX12__

#include <Interface/GpuDisplayAdapter.hpp>
#include <DirectX12/DirectX12Utility.hpp>


namespace CodeRed {

	class DirectX12DisplayAdapter final : public GpuDisplayAdapter {
	public:
		explicit DirectX12DisplayAdapter(
			const WRL::ComPtr<IDXGIAdapter1> &dxgi_adapter,
			const std::string &name,
			const UInt32 device_id,
			const UInt32 vendor_id);

		~DirectX12DisplayAdapter() = default;

		auto adapter() const noexcept -> WRL::ComPtr<IDXGIAdapter1> { return mAdapter; }
	private:
		WRL::ComPtr<IDXGIAdapter1> mAdapter;
	};
	
}

#endif
