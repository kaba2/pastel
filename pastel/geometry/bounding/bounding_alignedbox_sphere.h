// Description: Bounding aligned box of a sphere

#ifndef PASTELGEOMETRY_BOUNDING_ALIGNEDBOX_SPHERE_H
#define PASTELGEOMETRY_BOUNDING_ALIGNEDBOX_SPHERE_H

#include "pastel/geometry/shape/alignedbox.h"
#include "pastel/geometry/shape/sphere.h"

#include "pastel/math/affine_transformation.h"

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
		const AffineTransformation<Real>& transformation);

}

#include "pastel/geometry/bounding/bounding_alignedbox_sphere.hpp"

#endif
