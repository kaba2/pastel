#ifndef PASTEL_BOUNDING_SPHERE_BOX_HPP
#define PASTEL_BOUNDING_SPHERE_BOX_HPP

#include "pastel/geometry/bounding_sphere_box.h"

#include "pastel/sys/vector_tools.h"

namespace Pastel
{

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
