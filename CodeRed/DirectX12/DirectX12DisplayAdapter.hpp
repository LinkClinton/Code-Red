#pragma once

#include "../Interface/GpuDisplayAdapter.hpp"
#include "DirectX12Utility.hpp"

#ifdef __ENABLE__DIRECTX12__

namespace CodeRed {

	class DirectX12DisplayAdapter final : public GpuDisplayAdapter {
	public:
		explicit DirectX12DisplayAdapter(
			const WRL::ComPtr<IDXGIAdapter1> &dxgi_adapter,
			const std::string &name,
			const size_t device_id,
			const size_t vendor_id);

		~DirectX12DisplayAdapter() = default;

		auto adapter() const noexcept -> WRL::ComPtr<IDXGIAdapter1> { return mAdapter; }
	private:
		WRL::ComPtr<IDXGIAdapter1> mAdapter;
	};
	
}

#endif
