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
	Real convexPolygonDiameter2(
		const std::vector<Point<2, Real> >& pointSet)
	{
		const Integer2 antiPodal = convexPolygonAntipodal(pointSet);

		return distance2(pointSet[antiPodal[0]],
			pointSet[antiPodal[1]]);
	}

	template <typename Real>
	Real diameter2(
		const std::vector<Point<2, Real> >& pointSet)
	{
		const Integer2 antiPodal = antipodal(pointSet);

		return distance2(pointSet[antiPodal[0]],
			pointSet[antiPodal[1]]);
	}

	template <int N, typename Real>
	Real diameter2(
		const AlignedBox<N, Real>& box)
	{
		return dot(box.extent());
	}

	template <int N, typename Real>
	Real diameter(
		const Sphere<N, Real>& sphere)
	{
		return 2 * sphere.radius();
	}

	template <int N, typename Real>
	Real diameter2(
		const Sphere<N, Real>& sphere)
	{
		return 4 * sphere.radius() * sphere.radius();
	}

	template <int N, typename Real>
	Real diameter2(
		const Box<N, Real>& box)
	{
		return 2 * dot(box.width());
	}

}

#endif
