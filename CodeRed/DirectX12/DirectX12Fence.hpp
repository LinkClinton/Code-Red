#pragma once

#include "DirectX12Utility.hpp"
#include "../Interface/GpuFence.hpp"

#ifdef __ENABLE__DIRECTX12__

namespace CodeRed {

	class DirectX12Fence final : public GpuFence {
	public:
		explicit DirectX12Fence(
			const std::shared_ptr<GpuLogicalDevice>& device);

		~DirectX12Fence() = default;

		auto fence() const noexcept -> WRL::ComPtr<ID3D12Fence> { return mFence; }
	private:
		void wait(const WRL::ComPtr<ID3D12CommandQueue>& queue);
	private:
		friend class DirectX12CommandQueue;
		
		WRL::ComPtr<ID3D12Fence> mFence;

		UINT64 mFenceValue = 0;
	};	
}

#endif