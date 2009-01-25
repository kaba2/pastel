#ifndef PASTELGEOMETRY_BOX_TOOLS_HPP
#define PASTELGEOMETRY_BOX_TOOLS_HPP

#include "pastel/geometry/box_tools.h"

namespace Pastel
{

	template <int N, typename Real>
	Box<N, Real>& operator*=(Box<N, Real>& box,
		const AffineTransformation<N, Real>& rigidTransform)
	{
		box.setPosition(box.position() * rigidTransform);
		box.setRotation(box.rotation() * rigidTransform);

		return box;
	}

	template <int N, typename Real>
	Box<N, Real> operator*(const Box<N, Real>& box,
		const AffineTransformation<N, Real>& rigidTransform)
	{
		return Box<N, Real>(
			box.position() * rigidTransform,
			box.width(),
			box.rotation() * rigidTransform);
	}

}

#endif
