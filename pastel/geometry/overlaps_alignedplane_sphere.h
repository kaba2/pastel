// Description: Overlap tests between an aligned plane and a sphere

#ifndef PASTEL_OVERLAPS_ALIGNEDPLANE_SPHERE_H
#define PASTEL_OVERLAPS_ALIGNEDPLANE_SPHERE_H

#include "pastel/geometry/alignedplane.h"
#include "pastel/geometry/sphere.h"

namespace Pastel
{

	//! Tests if an aligned plane and a sphere overlap.

	/*!
	The sphere is considered to be closed and solid.
	*/

	template <int N, typename Real>
	bool overlaps(AlignedPlane<N, Real> const &alignedPlane,
		Sphere<N, Real> const &sphere);

	//! Tests if an aligned plane and a sphere overlap.

	/*!
	The sphere is considered to be closed and solid.
	'sphereOnPositiveSide' is filled with the
	information if the center of the sphere is
	on the positive side of the plane.
	*/

	template <int N, typename Real>
	bool overlaps(AlignedPlane<N, Real> const &alignedPlane,
		Sphere<N, Real> const &sphere,
		bool &sphereOnPositiveSide);

}

#include "pastel/geometry/overlaps_alignedplane_sphere.hpp"

#endif
