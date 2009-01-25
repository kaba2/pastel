/*!
\file
\brief A function for testing overlap between an alignedBox and a sphere.
*/

#ifndef PASTELGEOMETRY_OVERLAPS_ALIGNEDBOX_SPHERE_H
#define PASTELGEOMETRY_OVERLAPS_ALIGNEDBOX_SPHERE_H

#include "pastel/sys/mytypes.h"
#include "pastel/geometry/alignedbox.h"
#include "pastel/geometry/sphere.h"

namespace Pastel
{

	//! Tests if an alignedBox and a sphere overlap.

	/*!
	The alignedBox and the sphere are considered closed and
	solid.
	*/

	template <int N, typename Real>
	bool overlaps(
		const AlignedBox<N, Real>& alignedBox,
		const Sphere<N, Real>& sphere);

}

#include "pastel/geometry/overlaps_alignedbox_sphere.hpp"

#endif
