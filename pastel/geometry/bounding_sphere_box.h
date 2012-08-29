// Description: Bounding sphere of a box

#ifndef PASTELGEOMETRY_BOUNDING_SPHERE_BOX_H
#define PASTELGEOMETRY_BOUNDING_SPHERE_BOX_H

#include "pastel/geometry/sphere.h"
#include "pastel/geometry/box.h"

namespace Pastel
{

	//! Finds the minimum volume bounding sphere of a box.

	template <typename Real, int N>
	Sphere<Real, N> boundingSphere(
		const Box<Real, N>& box);

}

#include "pastel/geometry/bounding_sphere_box.hpp"

#endif
