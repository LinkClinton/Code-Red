#pragma once

#include <DirectX12/DirectX12Utility.hpp>
#include <Interface/GpuFrameBuffer.hpp>

#ifdef __ENABLE__DIRECTX12__

namespace CodeRed {

	class DirectX12FrameBuffer final : public GpuFrameBuffer {
	public:
		explicit DirectX12FrameBuffer(
			const std::shared_ptr<GpuLogicalDevice>& device,
			const std::shared_ptr<GpuTexture>& render_target,
			const std::shared_ptr<GpuTexture>& depth_stencil = nullptr);

		~DirectX12FrameBuffer() = default;

		auto rtvHeap() const noexcept -> WRL::ComPtr<ID3D12DescriptorHeap> { return mRenderTargetHeap; }

		auto dsvHeap() const noexcept -> WRL::ComPtr<ID3D12DescriptorHeap> { return mDepthStencilHeap; }
	private:
		WRL::ComPtr<ID3D12DescriptorHeap> mRenderTargetHeap;
		WRL::ComPtr<ID3D12DescriptorHeap> mDepthStencilHeap;
	};
	
}

#endif
