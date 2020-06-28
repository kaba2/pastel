#ifndef PASTELGEOMETRY_BOUNDING_SPHERE_POINTSET_HPP
#define PASTELGEOMETRY_BOUNDING_SPHERE_POINTSET_HPP

#include "pastel/geometry/bounding/bounding_sphere_pointset.h"

#include "pastel/math/statistic/point_mean.h"

#include "pastel/sys/vector/vector_tools.h"

namespace Pastel
{

	template <
		Point_Concept_ A_Point,
		Point_Concept_ B_Point
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

	template <
		PointSet_Concept_ PointSet
	>
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
