#pragma once

#include "../Enum/FilterOptions.hpp"
#include "../Enum/AddressMode.hpp"

#include <optional>

namespace CodeRed {

	struct SamplerInfo {
		FilterOptions Filter = FilterOptions::MinPointMagPointMipPoint;

		AddressMode AddressModeU = AddressMode::Clamp;
		AddressMode AddressModeV = AddressMode::Clamp;
		AddressMode AddressModeW = AddressMode::Clamp;

		UInt32 MaxAnisotropy = 1;
		Real Border[4] = { 0.0f, 0.0f, 0.0f, 1.0f };

		SamplerInfo() = default;

		explicit SamplerInfo(
			const FilterOptions filter,
			const AddressMode addressU = AddressMode::Clamp,
			const AddressMode addressV = AddressMode::Clamp,
			const AddressMode addressW = AddressMode::Clamp,
			const std::optional<Real[]>& border = std::nullopt) :
			Filter(filter),
			AddressModeU(addressU),
			AddressModeV(addressV),
			AddressModeW(addressW)
		{
			if (border.has_value() == false) return;

			Border[0] = border.value()[0];
			Border[1] = border.value()[1];
			Border[2] = border.value()[2];
			Border[3] = border.value()[3];
		}

		explicit SamplerInfo(
			const UInt32 maxAnisotropy,
			const AddressMode addressU = AddressMode::Clamp,
			const AddressMode addressV = AddressMode::Clamp,
			const AddressMode addressW = AddressMode::Clamp,
			const std::optional<Real[]>& border = std::nullopt) :
			Filter(FilterOptions::Anisotropy),
			AddressModeU(addressU),
			AddressModeV(addressV),
			AddressModeW(addressW),
			MaxAnisotropy(maxAnisotropy)
		{
			if (border.has_value() == false) return;

			Border[0] = border.value()[0];
			Border[1] = border.value()[1];
			Border[2] = border.value()[2];
			Border[3] = border.value()[3];
		}
	};
	
}