#ifndef PASTEL_OVERLAPS_PLANE_BOX_HPP
#define PASTEL_OVERLAPS_PLANE_BOX_HPP

#include "pastel/geometry/overlaps_plane_box.h"

#include "pastel/sys/mytypes.h"
#include "pastel/geometry/plane.h"
#include "pastel/geometry/box.h"
#include "pastel/sys/vector.h"
#include "pastel/math/matrix.h"

#include "pastel/geometry/distance_plane_point.h"

namespace Pastel
{

	template <int N, typename Real>
		bool overlaps(
			const Plane<N, Real>& plane,
			const Box<N, Real>& box)
	{
		const integer dimension = plane.dimension();
		PENSURE_OP(plane.dimension(), ==, box.dimension());

		// A plane and an box intersect if
		// the distance between an extremal
		// point of the box and the plane
		// is less or equal than the
		// distance between the center point
		// of the box and the plane.
		// An extremal point of a box
		// is such a point on the box that
		// it has the maximum distance to
		// the plane. It can be shown
		// that at least one of the corners
		// of the box is always an extremal
		// point.

		const Vector<N, Real>& planeNormal = 
			plane.normal();

		const Matrix<N, N, Real>& axes = box.rotation();

		// Find the distance between the center point
		// and an extremal point.

		// To maximize distance, we move
		// from the center using the box's
		// axes one by one. We choose
		// either -axis[i] or axis[i] depending
		// on which has the effect of increasing
		// the distance to the plane (this is
		// handled by the mabs()).

		Real d(0);
		for (integer i = 0;i < dimension;++i)
		{
			d += mabs(dot(axes[i], planeNormal));
		}

		// Calculate the distance of the center
		// point and the plane.

		const Real planeDistanceSquared = 
			distance2(plane, box.position());

		// If the distance of the center to the
		// plane is less than or equal to the distance
		// of the extremal point from the center,
		// then we have an intersection.

		return (planeDistanceSquared <= d);
	}

	template <int N, typename Real>
		bool overlaps(
			const Plane<N, Real>& plane,
			const Box<N, Real>& box,
			bool &boxOnPositiveSide)
	{
		// Check if the box's center point
		// is on the positive side of the plane.

		boxOnPositiveSide = dot(plane.normal(),
			box.position() - plane.position()) > 0;

		return Pastel::overlaps(plane, box);
	}

}

#endif
