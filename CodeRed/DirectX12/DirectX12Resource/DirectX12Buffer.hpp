#pragma once

#include "../../Interface/GpuResource/GpuBuffer.hpp"
#include "../DirectX12Utility.hpp"

#ifdef __ENABLE__DIRECTX12__

namespace CodeRed {

	class DirectX12Buffer final : public GpuBuffer {
	public:
		explicit DirectX12Buffer(
			const std::shared_ptr<GpuLogicalDevice>& device,
			const ResourceInfo info);

		~DirectX12Buffer() = default;

		auto mapMemory() const -> void* override;

		void unmapMemory() const override;
		
		auto buffer() const noexcept -> WRL::ComPtr<ID3D12Resource> { return mBuffer; }
	private:
		WRL::ComPtr<ID3D12Resource> mBuffer;
	};
	
}

#endif