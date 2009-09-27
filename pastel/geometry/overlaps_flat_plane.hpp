#ifndef PASTEL_OVERLAPS_FLAT_PLANE_HPP
#define PASTEL_OVERLAPS_FLAT_PLANE_HPP

#include "pastel/geometry/overlaps_flat_plane.h"

#include "pastel/geometry/flat.h"
#include "pastel/geometry/plane.h"
#include "pastel/geometry/line.h"

#include "pastel/sys/vector.h"
#include "pastel/sys/constants.h"

namespace Pastel
{

	template <typename Real, int N>
	bool overlaps(
		const Flat<Real, N, 2>& aPlane,
		const Plane<Real, N>& bPlane)
	{
		const Real divider(
			dot(aPlane[0], bPlane.normal()));
		const Vector<Real, N> delta(
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
