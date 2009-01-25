#ifndef PASTELGEOMETRY_OVERLAPS_ALIGNEDPLANE_SPHERE_HPP
#define PASTELGEOMETRY_OVERLAPS_ALIGNEDPLANE_SPHERE_HPP

#include "pastel/geometry/overlaps_alignedplane_sphere.h"

#include "pastel/sys/mytypes.h"
#include "pastel/geometry/alignedplane.h"
#include "pastel/geometry/sphere.h"

namespace Pastel
{

	template <int N, typename Real>
		bool overlaps(AlignedPlane<N, Real> const &alignedPlane,
			Sphere<N, Real> const &sphere)
	{
		const Real delta(sphere.position()[alignedPlane.axis()] -
			alignedPlane.position());

		return (std::abs(delta) <= sphere.radius());
	}

	template <int N, typename Real>
		bool overlaps(AlignedPlane<N, Real> const &alignedPlane,
			Sphere<N, Real> const &sphere,
		bool &sphereOnPositiveSide)
	{
		const Real delta(sphere.position()[alignedPlane.axis()] -
			alignedPlane.position());

		sphereOnPositiveSide = delta > 0;

		return std::abs(delta) <= sphere.radius();
	}

}

#endif
