// Description: Bounding sphere of a point-set

#ifndef PASTELGEOMETRY_BOUNDING_SPHERE_POINTSET_H
#define PASTELGEOMETRY_BOUNDING_SPHERE_POINTSET_H

#include "pastel/geometry/shape/sphere.h"

#include "pastel/sys/point/point_concept.h"
#include "pastel/sys/pointset/pointset_concept.h"

namespace Pastel
{

	//! Finds the minimum volume bounding sphere of two points.
	template <
		Point_Concept_ A_Point,
		Point_Concept_ B_Point
	>
	Sphere<Point_Real<A_Point>, Point_N<A_Point>::value> 
		boundingSphere(
			const A_Point& aPoint,
			const B_Point& bPoint);

	//! Finds a bounding sphere of a point set.
	template <PointSet_Concept_ PointSet>
	Sphere<PointSet_Real<PointSet>, PointSet_Dimension<PointSet>::value> 
		boundingSphere(const PointSet& pointSet);

}

#include "pastel/geometry/bounding/bounding_sphere_pointset.hpp"

#endif
