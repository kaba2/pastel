// Description: Overlap tests between a plane and a box

#ifndef PASTELGEOMETRY_OVERLAPS_PLANE_BOX_H
#define PASTELGEOMETRY_OVERLAPS_PLANE_BOX_H

#include "pastel/geometry/shape/plane.h"
#include "pastel/geometry/shape/box.h"
#include "pastel/geometry/distance/distance_plane_point.h"

#include "pastel/sys/mytypes.h"
#include "pastel/sys/vector.h"

#include "pastel/math/matrix/matrix.h"

namespace Pastel
{

	//! Tests if a plane and an box overlap.
	/*!
	Preconditions:
	plane.n() == box.n()

	Time complexity:
	O(n), where n is the dimension.

	Returns:
	Whether the plane and the box overlap.
	*/
	template <typename Real, int N>
	bool overlaps(
		const Plane<Real, N>& plane,
		const Box<Real, N>& box)
	{
		ENSURE_OP(plane.n(), ==, box.n());

		integer n = plane.n();

		// FIX: The topology of the box is not
		// handled correctly. The box is assumed
		// closed.

		// A plane and a box intersect if
		// the projection of the box onto the
		// plane normal intersects the projection
		// of the plane.
		
		// Find the radius of the projection
		// of the box.
		Real d = 0;
		for (integer i = 0;i < n;++i)
		{
			d += abs(dot(box.rotation()[i], plane.normal()));
		}

		// Calculate the distance of the center
		// point and the plane.
		Real planeDistance2 = 
			distance2(plane, box.position());

		// If the distance of the center to the
		// plane is less than or equal to the box's
		// projected radius, then we have an 
		// intersection.

		return (planeDistance2 <= square(d));
	}

	//! Tests if a plane and an box overlap.
	/*!
	boxOnPositiveSide:
	Set true, if the center point of the box
	is on the positive side of the plane.
	Otherwise set to false.

	Returns:
	overlaps(plane, box)
	*/
	template <typename Real, int N>
	bool overlaps(
		const Plane<Real, N>& plane,
		const Box<Real, N>& box,
		bool &boxOnPositiveSide)
	{
		ENSURE_OP(plane.n(), ==, box.n());

		// Check if the box's center point
		// is on the positive side of the plane.

		boxOnPositiveSide = dot(plane.normal(),
			box.position() - plane.position()) > 0;

		return Pastel::overlaps(plane, box);
	}

}

#endif
