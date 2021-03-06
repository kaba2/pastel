// Description: Distance between two points

#ifndef PASTELGEOMETRY_DISTANCE_POINT_POINT_H
#define PASTELGEOMETRY_DISTANCE_POINT_POINT_H

#include "pastel/math/norm/norm_concept.h"
#include "pastel/sys/point/point_concept.h"

// Template defaults

#include "pastel/math/norm/euclidean_norm.h"
#include "pastel/sys/indicator/all_indicator.h"

namespace Pastel
{

	//! Computes the distance between points.
	template <
		Point_Concept A_Point,
		Point_Concept B_Point, 
		typename Real = Point_Real<A_Point, B_Point>,
		typename... ArgumentSet
	>
	auto distance2(
		const A_Point& aPoint,
		const B_Point& bPoint,
		ArgumentSet&&... argumentSet)
	{
		auto&& norm = 
			PASTEL_ARG_C(norm, Euclidean_Norm<Real>(), Norm_Concept);

		auto distance = norm();
		using Distance = decltype(distance);
		
		auto&& keepGoing = PASTEL_ARG_C1(keepGoing, allIndicator(), Indicator_Concept, Distance);

		PENSURE_OP(dimension(aPoint), ==, dimension(bPoint));

		integer n = dimension(aPoint);
		for (integer i = 0;i < n && keepGoing(distance);++i)
		{
			distance.set(i, pointAxis(bPoint, i) - pointAxis(aPoint, i));
		}

		return distance;
	}

}

#endif
