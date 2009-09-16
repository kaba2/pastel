// Description: Closest points between an aligned box and a point

#ifndef PASTEL_CLOSEST_ALIGNEDBOX_POINT_H
#define PASTEL_CLOSEST_ALIGNEDBOX_POINT_H

#include "pastel/geometry/alignedbox.h"

namespace Pastel
{

	//! Computes closest points between an aligned box and a point.
	/*!
	Returns:
	A point C on the box such that C and 'point' are closest points.
	*/
	template <int N, typename Real>
	Vector<Real, N> closest(
		const AlignedBox<Real, N>& box,
		const Vector<Real, N>& point);

}

#include "pastel/geometry/closest_alignedbox_point.hpp"

#endif
