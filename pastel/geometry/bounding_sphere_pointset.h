// Description: Bounding sphere of a point-set

#ifndef PASTELGEOMETRY_BOUNDING_SPHERE_POINTSET_H
#define PASTELGEOMETRY_BOUNDING_SPHERE_POINTSET_H

#include "pastel/geometry/sphere.h"

#include "pastel/sys/range.h"

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

	template <typename Point_ConstRange, typename Locator>
	Sphere<typename Locator::Real, Locator::N> 
		boundingSphere(
		const Point_ConstRange& pointSet,
		const Locator& locator);

}

#include "pastel/geometry/bounding_sphere_pointset.hpp"

#endif
