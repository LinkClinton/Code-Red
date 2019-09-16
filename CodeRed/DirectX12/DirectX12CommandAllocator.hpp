#pragma once

#include "../Interface/GpuCommandAllocator.hpp"

#include "DirectX12Utility.hpp"

#ifdef __ENABLE__DIRECTX12__

namespace CodeRed {

	class DirectX12CommandAllocator final : public GpuCommandAllocator {
	public:
		explicit DirectX12CommandAllocator(
			const std::shared_ptr<GpuLogicalDevice> device);

		~DirectX12CommandAllocator() = default;

		auto allocator() const noexcept -> WRL::ComPtr<ID3D12CommandAllocator> { return mCommandAllocator; }
	protected:
		WRL::ComPtr<ID3D12CommandAllocator> mCommandAllocator;
	};
	
}

#endif