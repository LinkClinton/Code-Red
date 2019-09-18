#pragma once

#include <Shared/Utility.hpp>

namespace CodeRed {

	template<typename T>
	struct Extent2D {
		T Left = 0;
		T Top = 0;
		T Right = 0;
		T Bottom = 0;

		Extent2D() = default;

		explicit Extent2D(
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

		explicit Extent3D(
			const T left, const T top, const T front, 
			const T right, const T bottom, const T back) :
			Left(left), Top(top), Front(front),
			Right(right), Bottom(bottom), Back(back) {}
	};

	using UExtent2D = Extent2D<UInt32>;
	using UExtent3D = Extent3D<UInt32>;
}