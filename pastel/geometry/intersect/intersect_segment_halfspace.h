// Description: Intersection between a line segment and a half-space

#ifndef PASTELGEOMETRY_INTERSECT_SEGMENT_HALFSPACE_H
#define PASTELGEOMETRY_INTERSECT_SEGMENT_HALFSPACE_H

#include "pastel/geometry/shape/segment.h"
#include "pastel/geometry/shape/plane.h"

namespace Pastel
{

	template <typename Real, int N>
	bool intersect(
		const Segment<Real, N>& segment,
		const Plane<Real, N>& halfSpace,
		Vector<Real, 2>& hitList)
	{
		// Let P be a plane with normal n
		// and point q on it:
		//
		// P = {x in R^m: <x - q, n> = 0}
		//
		// Let S be a line segment between
		// points pStart and pEnd.
		// Let d = pEnd - start.
		//
		// The signed distance t from the point pStart
		// to the plane along the line induced by S
		// is solved by:
		//
		// <pStart + t d, n> = 0
		// => t = -<pStart, n> / <d, n>
		//
		// where t is measured in terms of the
		// length of S.
		//
		// To see on which side of the plane P
		// the points and A and B are, we compute:
		//
		// projectedStart = <pStart - q, n>
		// projectedEnd = <pEnd - q, n>
		//
		// S intersects P if and only if
		// projectedStart and projectedStart
		// have different signs.
		//
		// We then notice that:
		// projectedEnd - projectedStart 
		// = <pEnd - q, n> - <pStart - q, n>
		// = <pEnd - pStart, n>
		// = <d, n>
		//
		// Thus if S and P intersect, we
		// can compute t by:
		//
		// t = -projectedStart / (projectedEnd - projectedStart)

		Real projectedStart =
			dot(segment.start() - halfSpace.position(),
			halfSpace.normal());

		Real projectedEnd =
			dot(segment.end() - halfSpace.position(),
			halfSpace.normal());

		Real& tMin = hitList[0];
		Real& tMax = hitList[1];

		if (projectedStart < 0)
		{
			if (projectedEnd >= 0)
			{
				tMin = -projectedStart / 
					(projectedEnd - projectedStart);
				tMax = 1;
			}
			else
			{
				// Both segment points are
				// behind the half-space so
				// there is no intersection.

				return false;
			}
		}
		else
		{
			if (projectedEnd < 0)
			{
				tMin = 0;
				tMax = -projectedStart / 
					(projectedEnd - projectedStart);
			}
			else
			{
				// Both segment points are
				// in front of the half-space so
				// the whole segment is preserved.
				tMin = 0;
				tMax = 1;
			}
		}

		return true;
	}

}

#endif
