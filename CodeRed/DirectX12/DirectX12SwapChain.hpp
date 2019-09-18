#pragma once

#include <DirectX12/DirectX12Utility.hpp>
#include <Interface/GpuSwapChain.hpp>

#ifdef __ENABLE__DIRECTX12__

namespace CodeRed{

	class DirectX12SwapChain final : public GpuSwapChain {
	public:
		explicit DirectX12SwapChain(
			const std::shared_ptr<GpuLogicalDevice>& device,
			const WindowInfo& info,
			const PixelFormat& format,
			const size_t buffer_count = 2);

		~DirectX12SwapChain() = default;

		auto swapChain() const noexcept -> WRL::ComPtr<IDXGISwapChain3> { return mSwapChain; }
	private:
		WRL::ComPtr<ID3D12CommandQueue> mPresentQueue;
		WRL::ComPtr<IDXGISwapChain3> mSwapChain;
	};
	
}

#endif