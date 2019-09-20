#pragma once

#ifdef __CODE__RED__GLOBAL__INCLUDE__
#include <Shared/Enum/FilterOptions.hpp>
#include <Shared/Enum/AddressMode.hpp>
#include <Shared/Enum/BorderColor.hpp>
#else
#include "../Enum/FilterOptions.hpp"
#include "../Enum/AddressMode.hpp"
#include "../Enum/BorderColor.hpp"
#endif

namespace CodeRed {

	struct SamplerInfo {
		FilterOptions Filter = FilterOptions::MinPointMagPointMipPoint;

		AddressMode AddressModeU = AddressMode::Clamp;
		AddressMode AddressModeV = AddressMode::Clamp;
		AddressMode AddressModeW = AddressMode::Clamp;

		BorderColor Border;
		
		size_t MaxAnisotropy = 1;

		SamplerInfo() = default;

		explicit SamplerInfo(
			const FilterOptions filter,
			const AddressMode addressU = AddressMode::Clamp,
			const AddressMode addressV = AddressMode::Clamp,
			const AddressMode addressW = AddressMode::Clamp,
			const BorderColor border = BorderColor::TransparentBlack) :
			Filter(filter),
			AddressModeU(addressU),
			AddressModeV(addressV),
			AddressModeW(addressW),
			Border(border) {}

		explicit SamplerInfo(
			const UInt32 maxAnisotropy,
			const AddressMode addressU = AddressMode::Clamp,
			const AddressMode addressV = AddressMode::Clamp,
			const AddressMode addressW = AddressMode::Clamp,
			const BorderColor border = BorderColor::TransparentBlack) :
			Filter(FilterOptions::Anisotropy),
			AddressModeU(addressU),
			AddressModeV(addressV),
			AddressModeW(addressW),
			MaxAnisotropy(maxAnisotropy),
			Border(border) {}
		
	};
	
}