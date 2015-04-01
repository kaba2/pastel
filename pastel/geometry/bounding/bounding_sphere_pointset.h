// Description: Bounding sphere of a point-set

#ifndef PASTELGEOMETRY_BOUNDING_SPHERE_POINTSET_H
#define PASTELGEOMETRY_BOUNDING_SPHERE_POINTSET_H

#include "pastel/geometry/shape/sphere.h"

#include "pastel/sys/pointset/pointset_concept.h"

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
	template <typename PointSet>
	Sphere<PointSet_Real<PointSet>, PointSet_Dimension<PointSet>::value> 
		boundingSphere(PointSet pointSet);

}

#include "pastel/geometry/bounding/bounding_sphere_pointset.hpp"

#endif
