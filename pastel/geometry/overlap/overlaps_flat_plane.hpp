#ifndef PASTELGEOMETRY_OVERLAPS_FLAT_PLANE_HPP
#define PASTELGEOMETRY_OVERLAPS_FLAT_PLANE_HPP

#include "pastel/geometry/overlap/overlaps_flat_plane.h"

#include "pastel/geometry/shape/flat.h"
#include "pastel/geometry/shape/plane.h"
#include "pastel/geometry/shape/line.h"

#include "pastel/sys/vector.h"
#include "pastel/sys/math/constants.h"

namespace Pastel
{

	template <typename Real, integer N>
	bool overlaps(
		const Flat<Real, N, 2>& aPlane,
		const Plane<Real, N>& bPlane)
	{
		Real divider(
			dot(aPlane[0], bPlane.normal()));
		Vector<Real, N> delta(
			bPlane.position() - aPlane.position());

		// EPSILON
		if (divider == 0)
		{
			// EPSILON
			if (dot(delta, delta) == 0)
			{
				// Planes overlap
				return true;
			}

			return false;
		}

		return true;
	}

}

#endif
