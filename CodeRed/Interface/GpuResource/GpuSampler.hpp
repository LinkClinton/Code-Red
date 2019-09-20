#pragma once

#ifdef __CODE__RED__GLOBAL__INCLUDE__
#include <Shared/Information/SamplerInfo.hpp>
#include <Shared/Noncopyable.hpp>
#else
#include "../../Shared/Information/SamplerInfo.hpp"
#include "../../Shared/Noncopyable.hpp"
#endif

#include <memory>

namespace CodeRed {

	class GpuLogicalDevice;

	class GpuSampler : public Noncopyable {
	protected:
		explicit GpuSampler(
			const std::shared_ptr<GpuLogicalDevice>& device,
			const SamplerInfo& info);

		~GpuSampler() = default;
	public:
		auto info() const noexcept -> SamplerInfo { return mInfo; }
		
		auto filter() const noexcept -> FilterOptions { return mInfo.Filter; }

		auto addressU() const noexcept -> AddressMode { return mInfo.AddressModeU; }

		auto addressV() const noexcept -> AddressMode { return mInfo.AddressModeV; }

		auto addressW() const noexcept -> AddressMode { return mInfo.AddressModeW; }

		auto maxAnisotropy() const noexcept -> size_t { return mInfo.MaxAnisotropy; }

		auto border() const noexcept -> BorderColor { return mInfo.Border; }
	protected:
		std::shared_ptr<GpuLogicalDevice> mDevice;

		SamplerInfo mInfo;
	};
	
}