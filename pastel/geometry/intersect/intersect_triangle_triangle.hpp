#ifndef PASTELGEOMETRY_INTERSECT_TRIANGLE_TRIANGLE_HPP
#define PASTELGEOMETRY_INTERSECT_TRIANGLE_TRIANGLE_HPP

#include "pastel/geometry/intersect/intersect_triangle_triangle.h"
#include "pastel/geometry/shapes/segment.h"
#include "pastel/geometry/shapes/plane.h"
#include "pastel/geometry/shapes/flat.h"
#include "pastel/geometry/overlaps/overlaps_plane_triangle.h"
#include "pastel/geometry/intersect/intersect_flat_plane.h"
#include "pastel/geometry/intersect/intersect_alignedbox_alignedbox.h"
#include "pastel/geometry/shapes/linear_projection.h"

#include "pastel/sys/ensure.h"
#include "pastel/sys/vector_tools.h"

#include "pastel/sys/minmax.h"

namespace Pastel
{

	template <typename Real>
	bool intersect(
		const PASTEL_TRIANGLE(Real, 3)& aTriangle,
		const PASTEL_TRIANGLE(Real, 3)& bTriangle,
		Line<Real, 3>& intersectionLine,
		AlignedBox<Real, 1>& intersectionRange)
	{
		Plane<Real, 3> aPlane(
			aTriangle[0],
			cross(aTriangle[1] - aTriangle[0],
			aTriangle[2] - aTriangle[0]));

		if (!overlaps(aPlane, bTriangle))
		{
			return false;
		}

		Plane<Real, 3> bPlane(
			bTriangle[0],
			cross(bTriangle[1] - bTriangle[0],
			bTriangle[2] - bTriangle[0]));

		if (!overlaps(bPlane, aTriangle))
		{
			return false;
		}

		Flat<Real, 3, 2> aFlat;
		aFlat.setPosition(aTriangle[0]);
		aFlat[0] = aTriangle[1] - aTriangle[0];
		aFlat[1] = aTriangle[2] - aTriangle[0];

		// Since we already tested that the triangles
		// intersect each others planes, there
		// should be an intersection between the
		// planes.

		Line<Real, 3> line;
		bool intersected =
			Pastel::intersect(aFlat, bPlane, line);
		ASSERT(intersected);

		// Project the vertices on the intersection line.
		// See what their parametric position
		// is on that line..

		AlignedBox<Real, 1> aInterval = 
			projectAxis(aTriangle, line.direction());
		AlignedBox<Real, 1> bInterval = 
			projectAxis(bTriangle, line.direction());

		// If the parameter ranges intersect
		// then the triangles also intersect,
		// otherwise they don't.

		AlignedBox<Real, 1> sharedInterval;
		if (!intersect(aInterval, bInterval, sharedInterval))
		{
			return false;
		}

		intersectionLine = line;
		intersectionRange = sharedInterval - 
			Vector<Real, 1>(dot(line.position(), line.direction()));

		return true;
	}

}

#endif
