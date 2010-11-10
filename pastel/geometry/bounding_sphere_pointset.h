// Description: Bounding sphere of a point-set

#ifndef PASTEL_BOUNDING_SPHERE_POINTSET_H
#define PASTEL_BOUNDING_SPHERE_POINTSET_H

#include "pastel/geometry/sphere.h"

#include "pastel/sys/forwardrange.h"

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

	template <typename Point_ConstIterator, typename PointPolicy>
	Sphere<typename PointPolicy::Real, PointPolicy::N> 
		boundingSphere(
		const ForwardRange<Point_ConstIterator>& pointSet,
		const PointPolicy& pointPolicy);

}

#include "pastel/geometry/bounding_sphere_pointset.hpp"

#endif
