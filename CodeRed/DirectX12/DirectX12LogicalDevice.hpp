#pragma once

#include "../Interface/GpuLogicalDevice.hpp"

#include "DirectX12Utility.hpp"

#ifdef __ENABLE__DIRECTX12__

namespace CodeRed {

	class DirectX12LogicalDevice final : public GpuLogicalDevice {
	public:
		explicit DirectX12LogicalDevice(
			const std::shared_ptr<GpuDisplayAdapter>& adapter);

		~DirectX12LogicalDevice() = default;
		
		auto device() const noexcept -> WRL::ComPtr<ID3D12Device> { return mDevice; }

		auto commandAllocator() const noexcept -> WRL::ComPtr<ID3D12CommandAllocator> { return mCommandAllocator; }
	private:
		WRL::ComPtr<ID3D12CommandAllocator> mCommandAllocator;
		WRL::ComPtr<ID3D12Device> mDevice;
	};
	
}

#endif