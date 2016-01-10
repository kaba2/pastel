// Description: Distance between two points

#ifndef PASTELGEOMETRY_DISTANCE_POINT_POINT_H
#define PASTELGEOMETRY_DISTANCE_POINT_POINT_H

#include "pastel/math/normbijection/normbijection_concept.h"
#include "pastel/sys/point/point_concept.h"

// Template defaults

#include "pastel/math/normbijection/euclidean_normbijection.h"
#include "pastel/sys/indicator/all_indicator.h"

namespace Pastel
{

	//! Computes the norm-bijection distance between points.
	template <
		typename A_Point,
		typename B_Point, 
		// We need to use Point_Real_F instead of Point_Real to
		// avoid a bug in Visual Studio 2015 Update 1.
		typename Real = typename Point_Real_F<A_Point, B_Point>::type,
		typename... ArgumentSet,
		Requires<
			Models<A_Point, Point_Concept>,
			Models<B_Point, Point_Concept>
		> = 0>
	Real distance2(
		const A_Point& aPoint,
		const B_Point& bPoint,
		ArgumentSet&&... argumentSet)
	{
		auto&& normBijection = 
			PASTEL_ARG(
				normBijection, 
				[](){return Euclidean_NormBijection<Real>();},
				[](auto input) {return implicitArgument(Models<decltype(input), NormBijection_Concept>());}
			);
		
		auto&& keepGoing = 
			PASTEL_ARG(
				keepGoing, 
				[]() {return allIndicator();},
				[](auto input) {return Models<decltype(input), Indicator_Concept(Real)>();}
			);

		PENSURE_OP(dimension(aPoint), ==, dimension(bPoint));

		Real result = 0;
		
		integer n = dimension(aPoint);
		for (integer i = 0;i < n && keepGoing(result);++i)
		{
			result = normBijection.addAxis(result, 
				normBijection.signedAxis(
					pointAxis(bPoint, i) - pointAxis(aPoint, i)));
		}

		return result;
	}

}

#endif
