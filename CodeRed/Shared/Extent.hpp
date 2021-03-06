#pragma once

#include "Utility.hpp"

namespace CodeRed {

	template<typename T>
	struct Extent2D {
		T Left = 0;
		T Top = 0;
		T Right = 0;
		T Bottom = 0;

		Extent2D() = default;

		Extent2D(
			const T left, const T top,
			const T right, const T bottom) :
			Left(left), Top(top),
			Right(right), Bottom(bottom) {}
	};

	template<typename T>
	struct Extent3D {
		T Left = 0;
		T Top = 0;
		T Front = 0;
		T Right = 0;
		T Bottom = 0;
		T Back = 0;

		Extent3D() = default;

		Extent3D(
			const T left, const T top, const T front, 
			const T right, const T bottom, const T back) :
			Left(left), Top(top), Front(front),
			Right(right), Bottom(bottom), Back(back) {}

		auto width() const noexcept { return Right - Left; }

		auto height() const noexcept { return Bottom - Top; }

		auto depth() const noexcept { return Back - Front; }
	};

	using UExtent2D = Extent2D<size_t>;
	using UExtent3D = Extent3D<size_t>;
}