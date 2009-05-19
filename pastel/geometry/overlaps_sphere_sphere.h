/*!
\file
\brief A function for testing overlap between two spheres.
*/

#ifndef PASTEL_OVERLAPS_SPHERE_SPHERE_H
#define PASTEL_OVERLAPS_SPHERE_SPHERE_H

#include "pastel/geometry/sphere.h"

namespace Pastel
{

	//! Tests if two solid spheres intersect.

	/*!
	The spheres are considered to be closed and solid.
	*/

	template <int N, typename Real>
		bool overlaps(
			const Sphere<N, Real>& aSphere,
			const Sphere<N, Real>& bSphere);

}

#include "pastel/geometry/overlaps_sphere_sphere.hpp"

#endif
