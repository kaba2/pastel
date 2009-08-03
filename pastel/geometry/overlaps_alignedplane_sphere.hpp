#ifndef PASTEL_OVERLAPS_ALIGNEDPLANE_SPHERE_HPP
#define PASTEL_OVERLAPS_ALIGNEDPLANE_SPHERE_HPP

#include "pastel/geometry/overlaps_alignedplane_sphere.h"

#include "pastel/sys/mytypes.h"
#include "pastel/geometry/alignedplane.h"
#include "pastel/geometry/sphere.h"

namespace Pastel
{

	template <int N, typename Real>
		bool overlaps(AlignedPlane<Real, N> const &alignedPlane,
			Sphere<Real, N> const &sphere)
	{
		const Real delta(sphere.position()[alignedPlane.axis()] -
			alignedPlane.position());

		return (mabs(delta) <= sphere.radius());
	}

	template <int N, typename Real>
		bool overlaps(AlignedPlane<Real, N> const &alignedPlane,
			Sphere<Real, N> const &sphere,
		bool &sphereOnPositiveSide)
	{
		const Real delta(sphere.position()[alignedPlane.axis()] -
			alignedPlane.position());

		sphereOnPositiveSide = delta > 0;

		return mabs(delta) <= sphere.radius();
	}

}

#endif
