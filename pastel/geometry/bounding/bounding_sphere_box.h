// Description: Bounding sphere of a box

#ifndef PASTELGEOMETRY_BOUNDING_SPHERE_BOX_H
#define PASTELGEOMETRY_BOUNDING_SPHERE_BOX_H

#include "pastel/geometry/shape/sphere.h"
#include "pastel/geometry/shape/box.h"
#include "pastel/sys/vector/vector.h"

namespace Pastel
{

	//! Finds the minimum volume bounding sphere of a box.
	template <typename Real, int N>
	Sphere<Real, N> boundingSphere(
		const Box<Real, N>& box)
	{
		return Sphere<Real, N>(
			box.position(),
			std::sqrt(dot(box.width())));
	}

}

#endif
