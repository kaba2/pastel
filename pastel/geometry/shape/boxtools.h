// Description: Algorithms for Box's

#ifndef PASTELGEOMETRY_BOX_TOOLS_H
#define PASTELGEOMETRY_BOX_TOOLS_H

#include "pastel/geometry/shape/box.h"
#include "pastel/math/affine/affine_transformation.h"

namespace Pastel
{

	//! Transforma an box by the given transformation.
	/*!
	The matrix part of the transformation must be
	special orthogonal, otherwise the assumptions
	of the Box are broken down and some geometric
	algorithms work incorrectly.
	*/
	template <typename Real, int N>
	Box<Real, N>& operator*=(Box<Real, N>& box,
		const AffineTransformation<Real>& rigidTransform)
	{
		box.setPosition(box.position() * rigidTransform);
		box.setRotation(box.rotation() * rigidTransform);

		return box;
	}

	//! Returns an box transformed by the given transformation.
	/*!
	The matrix part of the transformation must be
	special orthogonal, otherwise the assumptions
	of the Box are broken down and some geometric
	algorithms work incorrectly.
	*/
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
