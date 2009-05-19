/*!
\file
\brief A functions for testing overlap between an box and a sphere.
*/

#ifndef PASTEL_OVERLAPBOXSPHERE_H
#define PASTEL_OVERLAPBOXSPHERE_H

#include "pastel/geometry/box.h"
#include "pastel/geometry/sphere.h"

namespace Pastel
{

	//! Tests if an box and a sphere overlap.

	/*!
	The box and the sphere are considered
	to be closed and solid.
	*/

	template <int N, typename Real>
	bool overlaps(
		const Box<N, Real>& box,
		const Sphere<N, Real>& sphere);

}

#include "pastel/geometry/overlaps_box_sphere.hpp"

#endif
