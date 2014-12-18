#ifndef PASTELGEOMETRY_BOX_TOOLS_HPP
#define PASTELGEOMETRY_BOX_TOOLS_HPP

#include "pastel/geometry/shapes/box_tools.h"

namespace Pastel
{

	template <typename Real, int N>
	Box<Real, N>& operator*=(Box<Real, N>& box,
		const AffineTransformation<Real>& rigidTransform)
	{
		box.setPosition(box.position() * rigidTransform);
		box.setRotation(box.rotation() * rigidTransform);

		return box;
	}

	template <typename Real, int N>
	Box<Real, N> operator*(const Box<Real, N>& box,
		const AffineTransformation<Real>& rigidTransform)
	{
		return Box<Real, N>(
			box.position() * rigidTransform,
			box.width(),
			box.rotation() * rigidTransform);
	}

}

#endif
