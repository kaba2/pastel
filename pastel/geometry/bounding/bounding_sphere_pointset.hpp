#ifndef PASTELGEOMETRY_BOUNDING_SPHERE_POINTSET_HPP
#define PASTELGEOMETRY_BOUNDING_SPHERE_POINTSET_HPP

#include "pastel/geometry/bounding/bounding_sphere_pointset.h"

#include "pastel/math/statistic/point_mean.h"

#include "pastel/sys/vector/vector_tools.h"

namespace Pastel
{

	template <
		typename Point,
		typename>
	Sphere<Point_Real<Point>, Point_Dimension<Point>::value> 
		boundingSphere(const Point& point)
	{
		return {pointAsVector(point), 0};
	}

	template <
		typename A_Point,
		typename B_Point,
		typename>
	Sphere<Point_Real<A_Point>, Point_Dimension<A_Point>::value> 
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
		typename PointSet,
		typename>
	Sphere<PointSet_Real<PointSet>, PointSet_Dimension<PointSet>::value> 
		boundingSphere(PointSet pointSet)
	{
		PASTEL_CONCEPT_CHECK(PointSet, PointSet_Concept);

		using Real = PointSet_Real<PointSet>;
		static PASTEL_CONSTEXPR int N = 
			PointSet_Dimension<PointSet>::value;

		// This function does not give the minimum volume
		// bounding sphere, but it does give something to
		// that direction, and is fast.

		integer n = pointSetSize(pointSet);

		Sphere<Real, N> result(n);
		if (pointSet.empty())
		{
			result.setRadius(0);
			return result;
		}

		// Compute the midpoint.
		Vector<Real, N> midPoint = 
			pointMean(pointSet);

		// Compute the maximum distance from the midpoint.
		Real maxDistance2 = 0;
		while(!pointSetEmpty(pointSet))
		{
			auto p = pointAsVector(pointSetGet(pointSet));
			
			Real currentDistance2 = dot(p - midPoint);
			if (currentDistance2 > maxDistance2)
			{
				maxDistance2 = currentDistance2;
			}

			pointSetPop(pointSet);
		}

		return Sphere<Real, N>(
			midPoint,
			std::sqrt(maxDistance2));
	}

}

#endif
