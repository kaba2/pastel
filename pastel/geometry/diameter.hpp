#ifndef PASTELGEOMETRY_DIAMETER_HPP
#define PASTELGEOMETRY_DIAMETER_HPP

#include "pastel/geometry/diameter.h"
#include "pastel/geometry/distance_point_point.h"
#include "pastel/geometry/antipodal.h"
#include "pastel/geometry/predicates.h"

#include "pastel/sys/mathfunctions.h"

namespace Pastel
{

	template <typename Real>
	Real convexPolygonDiameter(
		const std::vector<Point<2, Real> >& pointSet)
	{
		const Integer2 antiPodal = convexPolygonAntipodal(pointSet);

		return std::sqrt(distance2(pointSet[antiPodal[0]], 
			pointSet[antiPodal[1]]));
	}

	template <typename Real>
	Real diameter(
		const std::vector<Point<2, Real> >& pointSet)
	{
		const Integer2 antiPodal = antipodal(pointSet);

		return std::sqrt(distance2(pointSet[antiPodal[0]], 
			pointSet[antiPodal[1]]));
	}


}

#endif
