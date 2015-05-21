// Description: Bounding sphere of a point-set

#ifndef PASTELGEOMETRY_BOUNDING_SPHERE_POINTSET_H
#define PASTELGEOMETRY_BOUNDING_SPHERE_POINTSET_H

#include "pastel/geometry/shape/sphere.h"

#include "pastel/sys/point/point_concept.h"
#include "pastel/sys/pointset/pointset_concept.h"

namespace Pastel
{

	//! Finds the minimum volume bounding sphere of a point.
	template <
		typename Point,
		Requires<Models<Point, Point_Concept>> = 0
	>
	Sphere<Point_Real<Point>, Point_Dimension<Point>::value> 
		boundingSphere(const Point& point);

	//! Finds the minimum volume bounding sphere of two points.
	template <
		typename A_Point,
		typename B_Point,
		Requires<
			Models<A_Point, Point_Concept>,
			Models<B_Point, Point_Concept>
		> = 0
	>
	Sphere<Point_Real<A_Point>, Point_Dimension<A_Point>::value> 
		boundingSphere(
			const A_Point& aPoint,
			const B_Point& bPoint);

	//! Finds a bounding sphere of a point set.
	template <
		typename PointSet,
		Requires<
			Models<PointSet, PointSet_Concept>
		> = 0
	>
	Sphere<PointSet_Real<PointSet>, PointSet_Dimension<PointSet>::value> 
		boundingSphere(PointSet pointSet);

}

#include "pastel/geometry/bounding/bounding_sphere_pointset.hpp"

#endif
