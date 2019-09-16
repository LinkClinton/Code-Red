#pragma once


#include "../Shared/Enum/FilterOptions.hpp"
#include "../Shared/Enum/AddressMode.hpp"
#include "../Shared/Noncopyable.hpp"

#include <optional>

namespace CodeRed {

	class GpuSampler : public Noncopyable {
	protected:
		GpuSampler() = default;

		explicit GpuSampler(
			const FilterOptions filter,
			const AddressMode addressU = AddressMode::Clamp,
			const AddressMode addressV = AddressMode::Clamp,
			const AddressMode addressW = AddressMode::Clamp,
			const std::optional<Real[]>& border = std::nullopt) :
			mFilter(filter),
			mAddressModeU(addressU),
			mAddressModeV(addressV),
			mAddressModeW(addressW)
		{
			if (border.has_value() == false) return;

			mBorderColor[0] = border.value()[0];
			mBorderColor[1] = border.value()[1];
			mBorderColor[2] = border.value()[2];
			mBorderColor[3] = border.value()[3];
		}

		explicit GpuSampler(
			const UInt32 maxAnisotropy,
			const AddressMode addressU = AddressMode::Clamp,
			const AddressMode addressV = AddressMode::Clamp,
			const AddressMode addressW = AddressMode::Clamp,
			const std::optional<Real[]> &border = std::nullopt) :
			mFilter(FilterOptions::Anisotropy),
			mAddressModeU(addressU),
			mAddressModeV(addressV),
			mAddressModeW(addressW),
			mMaxAnisotropy(maxAnisotropy)
		{
			if (border.has_value() == false) return;

			mBorderColor[0] = border.value()[0];
			mBorderColor[1] = border.value()[1];
			mBorderColor[2] = border.value()[2];
			mBorderColor[3] = border.value()[3];
		}
	public:
		auto filter() const noexcept -> FilterOptions { return mFilter; }

		auto addressU() const noexcept -> AddressMode { return mAddressModeU; }

		auto addressV() const noexcept -> AddressMode { return mAddressModeV; }

		auto addressW() const noexcept -> AddressMode { return mAddressModeW; }

		auto maxAnisotropy() const noexcept -> UInt32 { return mMaxAnisotropy; }

		auto border() const noexcept -> const Real* { return mBorderColor; }
	protected:
		FilterOptions mFilter = FilterOptions::MinPointMagPointMipPoint;

		AddressMode mAddressModeU = AddressMode::Clamp;
		AddressMode mAddressModeV = AddressMode::Clamp;
		AddressMode mAddressModeW = AddressMode::Clamp;

		UInt32 mMaxAnisotropy = 1;
		Real mBorderColor[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
	};
	
}