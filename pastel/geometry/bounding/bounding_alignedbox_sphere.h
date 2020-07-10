// Description: Bounding aligned box of a sphere

#ifndef PASTELGEOMETRY_BOUNDING_ALIGNEDBOX_SPHERE_H
#define PASTELGEOMETRY_BOUNDING_ALIGNEDBOX_SPHERE_H

#include "pastel/geometry/shape/alignedbox.h"
#include "pastel/geometry/shape/sphere.h"
#include "pastel/math/affine/affine_transformation.h"
#include "pastel/sys/vector/vector_tools.h"

namespace Pastel
{

	//! Bounding aligned box of a sphere.
	template <typename Real, int N>
	AlignedBox<Real, N> boundingAlignedBox(
		const Sphere<Real, N>& sphere)
	{
		return AlignedBox<Real, N>(
			sphere.position() - sphere.radius(),
			sphere.position() + sphere.radius());
	}

	//! Bounding aligned box of an affinely transformed sphere.
	template <typename Real, int N>
	AlignedBox<Real, N> boundingAlignedBox(
		const Sphere<Real, N>& sphere,
		const AffineTransformation<Real, N, N>& transformation)
	{
		// One can show that:
		//
		// 1) The quadratic-form matrix of the ellipsoid
		// given by transforming an origin centered unit sphere 
		// with a linear transformation A is given by:
		// Q = A^-T A^-1
		// This is shown in 'ellipsoid.hpp'.
		//
		// 2) Given a quadratic-form matrix Q for an origin-centered 
		// ellipsoid, its axis-aligned bounding box is obtained
		// by +/- sqrt(diagonal(Q^-1)).
		// This is also shown in 'ellipsoid.hpp'.
		//
		// Combining, we obtain the axis-aligned bounding box by:
		// +/- sqrt(diagonal(A A^T)).

		// The bounding box is invariant w.r.t to the position of
		// the sphere. Thus the center of the sphere can simply be
		// transformed through the affine transformation.
		// The radius of the sphere simply scales the end-result
		// uniformly w.r.t the center of the sphere.

		// This is a prime example of the power of expression templates:
		// the complexity is just O(n^2) compared to O(n^3) if the
		// operations were actually carried out non-lazily.

		Vector<Real, N> radius =
			sqrt(
				diagonal(
					(transformation.matrix() * 
					transpose(transformation.matrix())).eval()
				)
			) * sphere.radius();

		Vector<Real, N> center =
			transformPoint(transformation, sphere.position());

		return AlignedBox<Real, N>(
			center - radius, 
			center + radius);
	}

}

#endif
