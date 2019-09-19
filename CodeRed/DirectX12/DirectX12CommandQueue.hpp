#pragma once

#ifdef __CODE__RED__GLOBAL__INCLUDE__
#include <DirectX12/DirectX12Utility.hpp>
#include <Interface/GpuCommandQueue.hpp>
#else
#include "DirectX12Utility.hpp"
#include "../Interface/GpuCommandQueue.hpp"
#endif

#ifdef __ENABLE__DIRECTX12__

namespace CodeRed {

	class DirectX12CommandQueue final : public GpuCommandQueue {
	public:
		explicit DirectX12CommandQueue(
			const std::shared_ptr<GpuLogicalDevice>& device);

		~DirectX12CommandQueue() = default;

		void execute(const std::vector<std::shared_ptr<GpuGraphicsCommandList>>& lists) override;

		void waitIdle() override;
		
		auto queue() const noexcept -> WRL::ComPtr<ID3D12CommandQueue> { return mCommandQueue; }
	protected:
		WRL::ComPtr<ID3D12CommandQueue> mCommandQueue;
	};
	
}

#endif