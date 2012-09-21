// Description: Algorithms for Box's

#ifndef PASTELGEOMETRY_BOX_TOOLS_H
#define PASTELGEOMETRY_BOX_TOOLS_H

#include "pastel/geometry/box.h"
#include "pastel/math/affine_transformation.h"

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
		const AffineTransformation<Real>& rigidTransform);

	//! Returns an box transformed by the given transformation.
	/*!
	The matrix part of the transformation must be
	special orthogonal, otherwise the assumptions
	of the Box are broken down and some geometric
	algorithms work incorrectly.
	*/

	template <typename Real, int N>
	Box<Real, N> operator*(const Box<Real, N>& box,
		const AffineTransformation<Real>& rigidTransform);

}

#include "pastel/geometry/box_tools.hpp"

#endif
