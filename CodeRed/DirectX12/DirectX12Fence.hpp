#pragma once

#include "../Interface/GpuFence.hpp"

#include "DirectX12Utility.hpp"

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