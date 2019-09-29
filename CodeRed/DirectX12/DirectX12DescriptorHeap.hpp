#pragma once

#include "../Interface/GpuDescriptorHeap.hpp"
#include "DirectX12Utility.hpp"

#ifdef __ENABLE__DIRECTX12__

namespace CodeRed {
	
	class DirectX12DescriptorHeap final : public GpuDescriptorHeap {
	public:
		explicit DirectX12DescriptorHeap(
			const std::shared_ptr<GpuLogicalDevice>& device,
			const std::shared_ptr<GpuResourceLayout>& resource_layout);

		~DirectX12DescriptorHeap() = default;

		void bindTexture(
			const size_t index, 
			const std::shared_ptr<GpuTexture>& texture) override;

		void bindBuffer(
			const size_t index, 
			const std::shared_ptr<GpuBuffer>& buffer) override;

		auto heap() const noexcept -> WRL::ComPtr<ID3D12DescriptorHeap> { return mDescriptorHeap; }
	private:
		WRL::ComPtr<ID3D12DescriptorHeap> mDescriptorHeap;

		size_t mDescriptorSize;
	};
	
}

#endif