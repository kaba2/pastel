// Description: Overlap tests between two line segments

#ifndef PASTELGEOMETRY_OVERLAPS_SEGMENT_SEGMENT_H
#define PASTELGEOMETRY_OVERLAPS_SEGMENT_SEGMENT_H

#include "pastel/geometry/shape/segment.h"
#include "pastel/geometry/predicates.h"
#include "pastel/geometry/shape/segment.h"

#include "pastel/sys/math_functions.h"
#include "pastel/sys/vector.h"
#include "pastel/sys/math/minmax.h"

namespace Pastel
{

	//! Tests if two segments overlap.
	/*!
	The segments are considered to be open. Only those
	intersections that result in a point are reported.
	This test is only reasonable in dimensions 1 and 2.
	In larger dimensions the probability of random segments
	intersecting is zero. Use distance
	calculations instead to tell the
	proximity of two segments in larger dimensions.
	*/
	template <typename Real>
	bool overlaps(
		const Segment<Real, 1>& aSegment,
		const Segment<Real, 1>& bSegment)
	{
		Real aMin(0);
		Real aMax(0);

		minMax(aSegment.start()[0], aSegment.end()[0],
			aMin, aMax);

		if ((aMin <= bSegment.start()[0] &&
			bSegment.start()[0] <= aMax) ||
			(aMin <= bSegment.end()[0] &&
			bSegment.end()[0] <= aMax))
		{
			return true;
		}

		return false;
	}

	//! Tests if two segments overlap.
	/*!
	The segments are considered to be open. Only those
	intersections that result in a point are reported.
	This test is only reasonable in dimensions 1 and 2.
	In larger dimensions the probability of random segments
	intersecting is zero. Use distance
	calculations instead to tell the
	proximity of two segments in larger dimensions.
	*/
	template <typename Real>
	bool overlaps(
		const Segment<Real, 2>& aSegment,
		const Segment<Real, 2>& bSegment)
	{
		// The segments intersect if and only if
		// 1) The endpoints of segment A are
		// on the different sides of the line B.
		// and
		// 2) The endpoints of segment B are
		// on the different sides of the line A.

		// Note: endpoint exactly on the line
		// is not considered an intersection.

		Plane<Real, 2> aPlane(
			aSegment.start(),
			cross(aSegment.end() - aSegment.start()));

		Plane<Real, 2> bPlane(
			bSegment.start(),
			cross(bSegment.end() - bSegment.start()));

		bool aCondition =
			(side(aSegment.start(), bPlane) *
			side(aSegment.end(), bPlane) < 0);

		bool bCondition =
			(side(bSegment.start(), aPlane) *
			side(bSegment.end(), aPlane) < 0);

		return aCondition && bCondition;
	}

}

#endif
