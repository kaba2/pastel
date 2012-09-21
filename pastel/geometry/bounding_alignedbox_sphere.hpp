#ifndef PASTELGEOMETRY_BOUNDING_ALIGNEDBOX_SPHERE_HPP
#define PASTELGEOMETRY_BOUNDING_ALIGNEDBOX_SPHERE_HPP

#include "pastel/geometry/bounding_alignedbox_sphere.h"

#include "pastel/sys/vector_tools.h"

namespace Pastel
{

	template <typename Real, int N>
	AlignedBox<Real, N> boundingAlignedBox(
		const Sphere<Real, N>& sphere)
	{
		const AlignedBox<Real, N> result(
			sphere.position() - sphere.radius(),
			sphere.position() + sphere.radius());

		return result;
	}

	template <typename Real, int N>
	AlignedBox<Real, N> boundingAlignedBox(
		const Sphere<Real, N>& sphere,
		const AffineTransformation<Real>& transformation)
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
		// +/- sqrt(diagonal(A A^T)). The results have to be modified
		// a bit because we use row vectors:
		// +/- sqrt(diagonal(A^T A))
		// The bounding box is invariant w.r.t to the position of
		// the sphere. Thus the center of the sphere can simply be
		// transfomed through the affine transformation.
		// The radius of the sphere simply scales the end-result
		// uniformly w.r.t the center of the sphere.

		// This is a prime example of the power of expression templates:
		// the complexity is just O(n^2) compared to O(n^3) if the
		// operations were actually carried out non-lazily.

		const Vector<Real, N> radius =
			sqrt(
			diagonal(
			transpose(transformation.matrix()) * 
			transformation.matrix())) * sphere.radius();

		const Vector<Real, N> center =
			transformPoint(transformation, sphere.position());

		const AlignedBox<Real, N> result(
			center - radius, center + radius);

		return result;
	}

}

#endif
