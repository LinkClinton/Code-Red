#pragma once

#include "../Interface/GpuTextureRef.hpp"
#include "DirectX12Utility.hpp"

#ifdef __ENABLE__DIRECTX12__

namespace CodeRed {

	class DirectX12TextureRef : public GpuTextureRef {
	public:
		explicit DirectX12TextureRef(
			const std::shared_ptr<GpuTexture>& texture,
			const TextureRefInfo& info);

		~DirectX12TextureRef() = default;

		auto desc() const noexcept -> D3D12_SHADER_RESOURCE_VIEW_DESC { return mDesc; }
	private:
		D3D12_SHADER_RESOURCE_VIEW_DESC mDesc;
	};
	
}

#endif