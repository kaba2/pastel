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
		Point_Concept_ A_Point,
		Point_Concept_ B_Point, 
		// We need to use Point_Real_F instead of Point_Real to
		// avoid a bug in Visual Studio 2015 Update 1.
		typename Real = typename Point_Real_F<A_Point, B_Point>::type,
		typename... ArgumentSet
	>
	auto distance2(
		const A_Point& aPoint,
		const B_Point& bPoint,
		ArgumentSet&&... argumentSet)
	{
		auto&& norm = 
			PASTEL_ARG_SC_(norm, Euclidean_Norm<Real>(), Norm_Concept_);

		auto distance = norm();
		using Distance = decltype(distance);
		
		auto&& keepGoing = 
			PASTEL_ARG_SC(keepGoing, allIndicator(), Indicator_Concept(Distance));

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
