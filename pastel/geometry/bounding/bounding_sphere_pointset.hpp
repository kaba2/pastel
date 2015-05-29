#ifndef PASTELGEOMETRY_BOUNDING_SPHERE_POINTSET_HPP
#define PASTELGEOMETRY_BOUNDING_SPHERE_POINTSET_HPP

#include "pastel/geometry/bounding/bounding_sphere_pointset.h"

#include "pastel/math/statistic/point_mean.h"

#include "pastel/sys/vector/vector_tools.h"

namespace Pastel
{

	template <
		typename Point,
		Requires<Models<Point, Point_Concept>>
	>
	Sphere<Point_Real<Point>, Point_N<Point>::value> 
		boundingSphere(const Point& point)
	{
		return {pointAsVector(point), 0};
	}

	template <
		typename A_Point,
		typename B_Point,
		Requires<
			Models<A_Point, Point_Concept>,
			Models<B_Point, Point_Concept>
		>
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
		typename PointSet,
		Requires<
			Models<PointSet, PointSet_Concept>
		>
	>
	Sphere<PointSet_Real<PointSet>, PointSet_Dimension<PointSet>::value> 
		boundingSphere(PointSet pointSet)
	{
		using Real = PointSet_Real<PointSet>;
		static constexpr int N = 
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
