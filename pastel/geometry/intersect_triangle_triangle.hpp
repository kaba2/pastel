#ifndef PASTEL_INTERSECT_TRIANGLE_TRIANGLE_HPP
#define PASTEL_INTERSECT_TRIANGLE_TRIANGLE_HPP

#include "pastel/geometry/intersect_triangle_triangle.h"
#include "pastel/geometry/segment.h"
#include "pastel/geometry/plane.h"
#include "pastel/geometry/flat.h"
#include "pastel/geometry/overlaps_plane_triangle.h"
#include "pastel/geometry/intersect_flat_plane.h"
#include "pastel/geometry/intersect_alignedbox_alignedbox.h"
#include "pastel/geometry/project_axis.h"

#include "pastel/sys/ensure.h"
#include "pastel/sys/vector_tools.h"

#include "pastel/math/minmax.h"

namespace Pastel
{

	template <typename Real>
	bool intersect(
		const Triangle<3, Real>& aTriangle,
		const Triangle<3, Real>& bTriangle,
		Line<3, Real>& intersectionLine,
		AlignedBox<1, Real>& intersectionRange)
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

		// Project the vertices on the intersection line.
		// See what their parametric position
		// is on that line..

		const AlignedBox<1, Real> aInterval = 
			projectAxis(aTriangle, line.direction());
		const AlignedBox<1, Real> bInterval = 
			projectAxis(bTriangle, line.direction());

		// If the parameter ranges intersect
		// then the triangles also intersect,
		// otherwise they don't.

		AlignedBox<1, Real> sharedInterval;
		if (!intersect(aInterval, bInterval, sharedInterval))
		{
			return false;
		}

		intersectionLine = line;
		intersectionRange = sharedInterval - 
			Vector<1, Real>(dot(asVector(line.position()), line.direction()));

		return true;
	}

}

#endif
