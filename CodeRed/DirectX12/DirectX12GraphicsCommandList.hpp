#pragma once

#include <Interface/GpuGraphicsCommandList.hpp>
#include <DirectX12/DirectX12Utility.hpp>

#ifdef __ENABLE__DIRECTX12__

namespace CodeRed {

	class DirectX12GraphicsCommandList final : GpuGraphicsCommandList {
	public:
		explicit DirectX12GraphicsCommandList(
			const std::shared_ptr<GpuLogicalDevice>& device,
			const std::shared_ptr<GpuCommandAllocator>& allocator);

		~DirectX12GraphicsCommandList() = default;

		auto commandList() const noexcept -> WRL::ComPtr<ID3D12GraphicsCommandList> { return mGraphicsCommandList; }
	protected:
		WRL::ComPtr<ID3D12GraphicsCommandList> mGraphicsCommandList;
	};
	
}

#endif