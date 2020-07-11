// Description: Distance between an aligned box and a point

#ifndef PASTELGEOMETRY_DISTANCE_ALIGNEDBOX_POINT_H
#define PASTELGEOMETRY_DISTANCE_ALIGNEDBOX_POINT_H

#include "pastel/sys/point/point_concept.h"
#include "pastel/math/distance/distance_concept.h"
#include "pastel/geometry/shape/alignedbox.h"
#include "pastel/geometry/distance/distance_alignedbox_point.h"
#include "pastel/geometry/distance/distance_point_point.h"

#include "pastel/math/norm/euclidean_norm.h"

namespace Pastel
{

	//! Distance between an aligned box and a point.
	/*!
	Preconditions:
	alignedBox.n() == point.n()

	Time complexity: O(alignedBox.n())

	It is assumed that the aligned box is solid.
	*/
	template <
		typename Real, 
		int N,
		Point_Concept Point,
		typename... ArgumentSet
	>
	requires 
		IsEqualDim<N, Point_N<Point>::value>
	auto distance2(
		const AlignedBox<Real, N>& alignedBox,
		const Point& point,
		ArgumentSet&&... argumentSet)
	{
		PENSURE_OP(alignedBox.n(), ==, dimension(point));

		auto&& norm = 
			PASTEL_ARG_C(norm, Euclidean_Norm<Real>(), Norm_Concept);
		
		// The distance computation between an AlignedBox and a point can
		// be decomposed into separate computations on each
		// coordinate axis. In this 1-dimensional world, the AlignedBox
		// degenerates into a range. For each axis we
		// calculate the squared distance of the point coordinate
		// to the AlignedBox coordinate range. If the point coordinate
		// is inside the AlignedBox coordinate range,
		// that particular distance is 0.
		// Finally, the 1-dimensional squared distances
		// are added together to obtain the dreal N-dimensional
		// squared distance.

		auto distance = norm();

		integer n = alignedBox.n();
		for (integer i = 0;i < n;++i)
		{
			Real x = pointAxis(point, i);
			if (x < alignedBox.min()[i])
			{
				// If the i:th point coordinate is
				// on the lesser side of the range,
				// base the distance calculation
				// on the range's minimum point.

				distance.set(i, alignedBox.min()[i] - x);
			}
			else if (x > alignedBox.max()[i])
			{
				// If the i:th point coordinate is
				// on the greater side of the range,
				// base the distance calculation
				// on the range's maximum point.

				distance.set(i, x - alignedBox.max()[i]);
			}
		}

		return distance;
	}

	//! Farthest distance between an aligned box and a point.
	/*!
	Preconditions:
	alignedBox.n() == point.n()

	Time complexity: O(alignedBox.n())

	It is assumed that the aligned box is solid.
	*/
	template <
		typename Real, 
		int N,
		Point_Concept Point,
		typename... ArgumentSet
	>
	requires 
		IsEqualDim<N, Point_N<Point>::value>
	auto farthestDistance2(
		const AlignedBox<Real, N>& alignedBox,
		const Point& point,
		ArgumentSet&&... argumentSet)
	{
		auto&& norm = 
			PASTEL_ARG_C(norm, Euclidean_Norm<Real>(), Norm_Concept);

		auto distance = norm();
		
		const integer n = alignedBox.n();
		for (integer i = 0;i < n;++i) {
			distance.set(i, 
				std::max(
					abs(alignedBox.min()[i] - pointAxis(point, i)),
					abs(alignedBox.max()[i] - pointAxis(point, i))
				)
			);
		}

		return distance;
	}

}

#endif
