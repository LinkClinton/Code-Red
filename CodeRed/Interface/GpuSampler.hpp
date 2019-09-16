#pragma once


#include "../Shared/Information/SamplerInfo.hpp"
#include "../Shared/Noncopyable.hpp"

#include <optional>
#include <memory>

namespace CodeRed {

	class GpuLogicalDevice;

	class GpuSampler : public Noncopyable {
	protected:
		explicit GpuSampler(
			const std::shared_ptr<GpuLogicalDevice> &device,
			const SamplerInfo& info) :
			mDevice(device),
			mInfo(info) {}

		~GpuSampler() = default;
	public:
		auto info() const noexcept -> SamplerInfo { return mInfo; }
		
		auto filter() const noexcept -> FilterOptions { return mInfo.Filter; }

		auto addressU() const noexcept -> AddressMode { return mInfo.AddressModeU; }

		auto addressV() const noexcept -> AddressMode { return mInfo.AddressModeV; }

		auto addressW() const noexcept -> AddressMode { return mInfo.AddressModeW; }

		auto maxAnisotropy() const noexcept -> UInt32 { return mInfo.MaxAnisotropy; }

		auto border() const noexcept -> const Real* { return mInfo.Border; }
	protected:
		std::shared_ptr<GpuLogicalDevice> mDevice;

		SamplerInfo mInfo;
	};
	
}