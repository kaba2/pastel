#ifndef PASTEL_INTERSECT_TRIANGLE_TRIANGLE_HPP
#define PASTEL_INTERSECT_TRIANGLE_TRIANGLE_HPP

#include "pastel/geometry/intersect_triangle_triangle.h"

#include "pastel/sys/ensure.h"

#include "pastel/geometry/segment.h"
#include "pastel/geometry/plane.h"
#include "pastel/geometry/flat.h"
#include "pastel/sys/vector_tools.h"

#include "pastel/geometry/overlaps_plane_triangle.h"
#include "pastel/geometry/intersect_flat_plane.h"

#include "pastel/math/minmax.h"

namespace Pastel
{

	template <typename Real>
	bool intersect(
		const Triangle<3, Real>& aTriangle,
		const Triangle<3, Real>& bTriangle,
		Segment<3, Real>& segment)
	{
		const Plane<3, Real> aPlane(
			aTriangle[0],
			cross(aTriangle[1] - aTriangle[0],
			aTriangle[2] - aTriangle[0]));

		if (!overlaps(aPlane, bTriangle))
		{
			return false;
		}

		const Plane<3, Real> bPlane(
			bTriangle[0],
			cross(bTriangle[1] - bTriangle[0],
			bTriangle[2] - bTriangle[0]));

		if (!overlaps(bPlane, aTriangle))
		{
			return false;
		}

		Flat<3, Real, 2> aFlat;
		aFlat.setPosition(aTriangle[0]);
		aFlat[0] = aTriangle[1] - aTriangle[0];
		aFlat[1] = aTriangle[2] - aTriangle[0];

		// Since we already tested that the triangles
		// intersect each others planes, there
		// should be an intersection between the
		// planes.

		Line<3, Real> line;
		bool intersected =
			Pastel::intersect(aFlat, bPlane, line);
		ASSERT(intersected);

		// Project the a vertices on the intersection line.
		// See what their parametric position
		// is on that line..

		const Real a1(dot(line.direction(),
			aTriangle[0] - line.position()));
		const Real a2(dot(line.direction(),
			aTriangle[1] - line.position()));
		const Real a3(dot(line.direction(),
			aTriangle[2] - line.position()));

		// Find out the min-max range of the
		// a parameters.

		Real aMin(0);
		Real aMax(0);

		minMax(a1, a2, a3, aMin, aMax);

		// Project the b vertices on the intersection line.
		// See what their parametric position
		// is on that line..

		const Real b1(dot(line.direction(),
			bTriangle[0] - line.position()));
		const Real b2(dot(line.direction(),
			bTriangle[1] - line.position()));
		const Real b3(dot(line.direction(),
			bTriangle[2] - line.position()));

		// Find out the min-max range of the
		// b parameters.

		Real bMin(0);
		Real bMax(0);

		minMax(b1, b2, b3, bMin, bMax);

		// If the parameter ranges intersect
		// then the triangles also intersect,
		// otherwise they don't.

		bool rangesIntersect =
			(aMin <= bMin && bMin <= aMax) ||
			(aMin <= bMax && bMax <= aMax);

		if (rangesIntersect)
		{
			// Find out the intersection range.
			// TODO!!!
		}

		ENSURE(false);

		return rangesIntersect;
	}

}

#endif
