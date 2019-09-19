#pragma once

#ifdef __CODE__RED__GLOBAL__INCLUDE__
#include <Interface/GpuResource/GpuSampler.hpp>
#include <DirectX12/DirectX12Utility.hpp>
#else
#include "../../Interface/GpuResource/GpuSampler.hpp"
#include "../DirectX12Utility.hpp"
#endif

#ifdef __ENABLE__DIRECTX12__

namespace CodeRed {

	class DirectX12Sampler final : public GpuSampler {
	public:
		explicit DirectX12Sampler(
			const std::shared_ptr<GpuLogicalDevice>& device,
			const SamplerInfo& info);

		~DirectX12Sampler() = default;

		auto constexpr sampler() const noexcept -> D3D12_STATIC_SAMPLER_DESC { return mSampler; }
	protected:
		D3D12_STATIC_SAMPLER_DESC mSampler;
	};
	
}

#endif