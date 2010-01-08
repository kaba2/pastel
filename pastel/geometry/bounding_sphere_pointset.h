// Description: Bounding sphere of a point-set

#ifndef PASTEL_BOUNDING_SPHERE_POINTSET_H
#define PASTEL_BOUNDING_SPHERE_POINTSET_H

#include "pastel/geometry/sphere.h"

namespace Pastel
{

	//! Finds the minimum volume bounding sphere of a point.

	template <typename Real, int N>
	Sphere<Real, N> boundingSphere(
		const Vector<Real, N>& aPoint);

	//! Finds the minimum volume bounding sphere of two points.

	template <typename Real, int N>
	Sphere<Real, N> boundingSphere(
		const Vector<Real, N>& aPoint,
		const Vector<Real, N>& bPoint);

	//! Finds a bounding sphere of a point set.
	/*!
	The InputIterator must dereference to Vector<Real, N>.
	*/

	template <typename Real, int N, typename InputIterator, typename PositionFunctor>
	Sphere<Real, N> boundingSphere(
		const InputIterator& from,
		const InputIterator& to,
		const PositionFunctor& positionFunctor);

	//! Finds a bounding sphere of a point set.
	/*!
	The InputIterator must dereference to Vector<Real, N>.
	*/

	template <typename Real, int N, typename InputIterator>
	Sphere<Real, N> boundingSphere(
		const InputIterator& from,
		const InputIterator& to);

}

#include "pastel/geometry/bounding_sphere_pointset.hpp"

#endif
