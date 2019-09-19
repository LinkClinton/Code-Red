#pragma once

#ifdef __CODE__RED__GLOBAL__INCLUDE__
#include <Interface/GpuResourceLayout.hpp>
#include <DirectX12/DirectX12Utility.hpp>
#else
#include "../Interface/GpuResourceLayout.hpp"
#include "DirectX12Utility.hpp"
#endif

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
	protected:
		WRL::ComPtr<ID3D12RootSignature> mRootSignature;
	};
	
}

#endif