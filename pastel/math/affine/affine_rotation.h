// Description: Rotating affine transformation
// Documentation: affine_algorithms.txt

#ifndef PASTELMATH_AFFINE_ROTATION_H
#define PASTELMATH_AFFINE_ROTATION_H

#include "pastel/math/affine/affine_transformation.h"

namespace Pastel
{

	//! Returns a rotating affine transformation.
	/*!
	The rotation is from the standard basis axis 
	e_i towards axis e_j.

	n:
	The dimension of the rotation.

	i, j:
	The standard basis axes e_i and e_j to use as 
	the rotation plane.

	angle:
	The rotation angle.

	Preconditions:
	n >= 0
	0 <= i < n
	0 <= j < n
	*/
	template <typename Real>
	AffineTransformation<Real> affineRotation(
		integer n,
		integer i, integer j,
		const NoDeduction<Real>& angle);

}

#include "pastel/math/affine/affine_rotation.hpp"

#endif
