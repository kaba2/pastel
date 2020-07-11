// Description: Bounding sphere of a point-set

#ifndef PASTELGEOMETRY_BOUNDING_SPHERE_POINTSET_H
#define PASTELGEOMETRY_BOUNDING_SPHERE_POINTSET_H

#include "pastel/geometry/shape/sphere.h"

#include "pastel/sys/point/point_concept.h"
#include "pastel/sys/pointset/pointset_concept.h"
#include "pastel/sys/vector/vector.h"
#include "pastel/math/statistic/point_mean.h"

namespace Pastel
{

	//! Finds the minimum volume bounding sphere of two points.
	template <
		Point_Concept A_Point,
		Point_Concept B_Point
	>
	Sphere<Point_Real<A_Point>, Point_N<A_Point>::value> 
		boundingSphere(
			const A_Point& aPoint,
			const B_Point& bPoint)
	{
		return 
			{
				pointAsVector(aPoint) / 2 + pointAsVector(bPoint) / 2,
				norm(evaluate(pointAsVector(bPoint) - pointAsVector(aPoint))) / 2
			};
	}

	//! Finds a bounding sphere of a point set.
	template <PointSet_Concept PointSet>
	Sphere<PointSet_Real<PointSet>, PointSet_Dimension<PointSet>::value> 
		boundingSphere(const PointSet& pointSet)
	{
		using Real = PointSet_Real<PointSet>;
		static constexpr int N = 
			PointSet_Dimension<PointSet>::value;

		// This function does not give the minimum volume
		// bounding sphere, but it does give something to
		// that direction, and is fast.

		integer n = pointSetDimension(pointSet);

		Sphere<Real, N> result(n);
		if (emptySet(pointSet))
		{
			result.setRadius(0);
			return result;
		}

		// Compute the midpoint.
		Vector<Real, N> midPoint = 
			pointMean(pointSet);

		// Compute the maximum distance from the midpoint.
		Real maxDistance2 = 0;
		RANGES_FOR(auto&& point, pointSet)
		{
			Real currentDistance2 = dot(pointAsVector(point) - midPoint);
			if (currentDistance2 > maxDistance2)
			{
				maxDistance2 = currentDistance2;
			}
		}

		return Sphere<Real, N>(
			midPoint,
			std::sqrt(maxDistance2));
	}

}

#endif
