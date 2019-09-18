#pragma once

#include <DirectX12/DirectX12Utility.hpp>
#include <Interface/GpuFence.hpp>

#ifdef __ENABLE__DIRECTX12__

namespace CodeRed {

	class DirectX12Fence : public GpuFence {
	public:
		explicit DirectX12Fence(
			const std::shared_ptr<GpuLogicalDevice>& device);

		~DirectX12Fence() = default;
	protected:
		WRL::ComPtr<ID3D12Fence> mFence;

		UINT64 mFenceValue = 0;
	};	
}

#endif