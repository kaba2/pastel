#ifndef PASTELGEOMETRY_OVERLAPS_FLAT_PLANE_HPP
#define PASTELGEOMETRY_OVERLAPS_FLAT_PLANE_HPP

#include "pastel/geometry/overlaps_flat_plane.h"

#include "pastel/geometry/flat.h"
#include "pastel/geometry/plane.h"
#include "pastel/geometry/line.h"
#include "pastel/sys/vector.h"
#include "pastel/sys/point.h"
#include "pastel/sys/constants.h"

namespace Pastel
{

	template <int N, typename Real>
	bool overlaps(
		const Flat<N, Real, 2>& aPlane,
		const Plane<N, Real>& bPlane)
	{
		const Real divider(
			dot(aPlane[0], bPlane.normal()));
		const Vector<N, Real> delta(
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
