// Description: Bounding aligned box of a sphere
// Documentation: bounding.txt

#ifndef PASTEL_BOUNDING_ALIGNEDBOX_SPHERE_H
#define PASTEL_BOUNDING_ALIGNEDBOX_SPHERE_H

#include "pastel/geometry/alignedbox.h"
#include "pastel/geometry/sphere.h"

#include "pastel/math/affinetransformation.h"

namespace Pastel
{

	//! Bounding aligned box of a sphere.

	template <typename Real, int N>
	AlignedBox<Real, N> boundingAlignedBox(
		const Sphere<Real, N>& sphere);

	//! Bounding aligned box of an affinely transformed sphere.

	template <typename Real, int N>
	AlignedBox<Real, N> boundingAlignedBox(
		const Sphere<Real, N>& sphere,
		const AffineTransformation<Real, N>& transformation);

}

#include "pastel/geometry/bounding_alignedbox_sphere.hpp"

#endif
