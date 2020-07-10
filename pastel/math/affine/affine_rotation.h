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
	template <typename Real, int N>
	AffineTransformation<Real, N, N> affineRotation(
		integer n,
		integer i, integer j,
		const NoDeduction<Real>& angle)
	{
		PENSURE_OP(n, >=, 2);
		PENSURE_OP(i, >=, 0);
		PENSURE_OP(i, <, n);
		PENSURE_OP(j, >=, 0);
		PENSURE_OP(j, <, n);

		Real c = std::cos(angle);
		Real s = std::sin(angle);

		// For example, when
		// n = 2, i = 0, j = 1:
		//
		// [c -s]
		// [s  c]
		//
		// which is the counter-clockwise rotation
		// rotation of X to Y.
		//
		// If instead i = 1, and j = 0, then it is
		// the clockwise-rotation of Y to X.

		AffineTransformation<Real, N, N> result(n);
		
		result.matrix()(i, i) = c;
		result.matrix()(i, j) = -s;
		result.matrix()(j, i) = s;
		result.matrix()(j, j) = c;

		return result;
	}

}

#endif
