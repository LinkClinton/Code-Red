#pragma once

#include "../Interface/GpuResourceLayout.hpp"
#include "../Shared/IdentityAllocator.hpp"
#include "DirectX12Utility.hpp"

#include <map>

#ifdef __ENABLE__DIRECTX12__

namespace CodeRed {

	class DirectX12ResourceLayout final : public GpuResourceLayout {
	public:
		explicit DirectX12ResourceLayout(
			const std::shared_ptr<GpuLogicalDevice>& device,
			const std::vector<ResourceLayoutElement>& elements,
			const std::vector<SamplerLayoutElement>& samplers);

		~DirectX12ResourceLayout() = default;

		auto rootSignature() const noexcept -> WRL::ComPtr<ID3D12RootSignature> { return mRootSignature; }
	private:
		WRL::ComPtr<ID3D12RootSignature> mRootSignature;
	};
	
}

#endif