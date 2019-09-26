#pragma once

#ifdef __CODE__RED__GLOBAL__INCLUDE__
#include <DirectX12/DirectX12Utility.hpp>
#include <Interface/GpuSwapChain.hpp>
#else
#include "DirectX12Utility.hpp"
#include "../Interface/GpuSwapChain.hpp"
#endif

#ifdef __ENABLE__DIRECTX12__

namespace CodeRed{

	class DirectX12SwapChain final : public GpuSwapChain {
	public:
		explicit DirectX12SwapChain(
			const std::shared_ptr<GpuLogicalDevice>& device,
			const std::shared_ptr<GpuCommandQueue>& queue,
			const WindowInfo& info,
			const PixelFormat& format,
			const size_t buffer_count = 2);

		~DirectX12SwapChain() = default;

		void resize(const size_t width, const size_t height) override;

		void present() override;

		auto currentBufferIndex() const -> size_t override;
		
		auto swapChain() const noexcept -> WRL::ComPtr<IDXGISwapChain3> { return mSwapChain; }
	private:
		WRL::ComPtr<IDXGISwapChain3> mSwapChain;
	};
	
}

#endif