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
			const std::vector<ResourceLayoutElement>& elements = {},
			const std::vector<SamplerLayoutElement>& samplers = {},
			const std::optional<Constant32Bits>& constant32Bits = std::nullopt);

		~DirectX12ResourceLayout() = default;

		auto rootSignature() const noexcept -> WRL::ComPtr<ID3D12RootSignature> { return mRootSignature; }

		auto elementsIndex() const noexcept -> size_t { return mElementsIndex; }

		auto constant32BitsIndex() const noexcept -> size_t { return mConstant32BitsIndex; }
	private:
		WRL::ComPtr<ID3D12RootSignature> mRootSignature;

		size_t mElementsIndex = 0;
		size_t mConstant32BitsIndex = 1;
	};
	
}

#endif