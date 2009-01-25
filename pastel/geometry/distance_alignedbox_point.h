/*!
\file
\brief A function for computing the distance between an aligned box and a point.
*/

#ifndef PASTELGEOMETRY_DISTANCE_ALIGNEDBOX_POINT_H
#define PASTELGEOMETRY_DISTANCE_ALIGNEDBOX_POINT_H

#include "pastel/geometry/alignedbox.h"
#include "pastel/sys/point.h"

namespace Pastel
{

	//! Calculates the squared distance between an aligned box and a point.

	template <int N, typename Real>
		Real distance2(
			const AlignedBox<N, Real>& alignedBox,
			const Point<N, Real>& point);

}

#include "pastel/geometry/distance_alignedbox_point.hpp"

#endif
