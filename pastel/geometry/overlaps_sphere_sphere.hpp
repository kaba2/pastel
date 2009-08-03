#ifndef PASTEL_OVERLAPS_SPHERE_SPHERE_HPP
#define PASTEL_OVERLAPS_SPHERE_SPHERE_HPP

#include "pastel/geometry/overlaps_sphere_sphere.h"

#include "pastel/sys/mytypes.h"
#include "pastel/geometry/sphere.h"
#include "pastel/sys/vector.h"

namespace Pastel
{

	template <int N, typename Real>
		bool overlaps(
			const Sphere<N, Real>& aSphere,
			const Sphere<N, Real>& bSphere)
	{
		// Two spheres intersect if the distance
		// between their center points is
		// less than or equal to the sum of their radii.

		const Vector<Real, N> delta(
			bSphere.position() - aSphere.position());

		const Real centerDistance2(dot(delta, delta));
		const Real radiusSum(aSphere.radius() + bSphere.radius());

		return centerDistance2 <= radiusSum * radiusSum;
	}

}

#endif
