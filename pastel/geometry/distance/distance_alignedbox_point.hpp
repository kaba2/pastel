#ifndef PASTELGEOMETRY_DISTANCE_ALIGNEDBOX_POINT_HPP
#define PASTELGEOMETRY_DISTANCE_ALIGNEDBOX_POINT_HPP

#include "pastel/geometry/distance/distance_alignedbox_point.h"
#include "pastel/geometry/distance/distance_point_point.h"

#include "pastel/math/normbijection/euclidean_normbijection.h"

namespace Pastel
{

	template <
		typename Real, 
		int N,
		typename Point,
		typename... ArgumentSet,
		Requires<
			Models<Point, Point_Concept>,
			EqualDimension<Integer<N>, Point_N<Point>>
		>>
	Real distance2(
		const AlignedBox<Real, N>& alignedBox,
		const Point& point,
		ArgumentSet&&... argumentSet)
	{
		PENSURE_OP(alignedBox.n(), ==, dimension(point));

		auto&& normBijection = 
			PASTEL_ARG(
				normBijection,
				[](){return Euclidean_NormBijection<Real>();},
				[](auto input) {return implicitArgument(Models<decltype(input), NormBijection_Concept>());}
			);
		
		// The distance computation between an AlignedBox and a point can
		// be decomposed into separate computations on each
		// coordinate axis. In this 1-dimensional world, the AlignedBox
		// degenerates into a range. For each axis we
		// calculate the squared distance of the point coordinate
		// to the AlignedBox coordinate range. If the point coordinate
		// is inside the AlignedBox coordinate range,
		// that particular distance is 0.
		// Finally, the 1-dimensional squared distances
		// are added together to obtain the real N-dimensional
		// squared distance.

		Real result = 0;

		integer n = alignedBox.n();
		for (integer i = 0;i < n;++i)
		{
			if (pointAxis(point, i) < alignedBox.min()[i])
			{
				// If the i:th point coordinate is
				// on the lesser side of the range,
				// base the distance calculation
				// on the range's minimum point.

				result = normBijection.addAxis(
					result,
					normBijection.axis(alignedBox.min()[i] - pointAxis(point, i)));
			}
			else if (pointAxis(point, i) > alignedBox.max()[i])
			{
				// If the i:th point coordinate is
				// on the greater side of the range,
				// base the distance calculation
				// on the range's maximum point.

				result = normBijection.addAxis(
					result,
					normBijection.axis(pointAxis(point, i) - alignedBox.max()[i]));
			}
		}

		return result;
	}

	template <
		typename Real, 
		int N,
		typename Point,
		typename... ArgumentSet,
		Requires<
			Models<Point, Point_Concept>,
			EqualDimension<Integer<N>, Point_N<Point>>
		>>
	Real farthestDistance2(
		const AlignedBox<Real, N>& alignedBox,
		const Point& point,
		ArgumentSet&&... argumentSet)
	{
		auto&& normBijection = 
			PASTEL_ARG(
				normBijection,
				[](){return Euclidean_NormBijection<Real>();},
				[](auto input) {return implicitArgument(Models<decltype(input), NormBijection_Concept>());}
			);

		Real result = 0;

		integer n = alignedBox.n();
		for (integer i = 0;i < n;++i)
		{
			result = normBijection.addAxis(
				result,
				normBijection.axis(
					std::max(
						abs(alignedBox.min()[i] - pointAxis(point, i)),
						abs(alignedBox.max()[i] - pointAxis(point, i))
					)
				));
		}

		return result;
	}

}

#endif
