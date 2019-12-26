#pragma once

#include "../../Interface/GpuResource/GpuTexture.hpp"
#include "../DirectX12Utility.hpp"

#ifdef __ENABLE__DIRECTX12__

namespace CodeRed {

	class DirectX12Texture final : public GpuTexture {
	public:
		explicit DirectX12Texture(
			const std::shared_ptr<GpuLogicalDevice>& device,
			const ResourceInfo& info);

		explicit DirectX12Texture(
			const std::shared_ptr<GpuLogicalDevice>& device,
			const WRL::ComPtr<ID3D12Resource>& texture,
			const ResourceInfo& info);

		~DirectX12Texture() = default;
		
		auto texture() const noexcept -> WRL::ComPtr<ID3D12Resource> { return mTexture; }
	private:
		WRL::ComPtr<ID3D12Resource> mTexture;
	};
	
}

#endif
