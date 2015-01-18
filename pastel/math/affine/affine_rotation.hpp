#ifndef PASTELMATH_AFFINE_ROTATION_HPP
#define PASTELMATH_AFFINE_ROTATION_HPP

#include "pastel/math/affine/affine_rotation.h"

namespace Pastel
{

	template <typename Real>
	AffineTransformation<Real> affineRotation(
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

		AffineTransformation<Real> result(n);
		
		result.matrix()(i, i) = c;
		result.matrix()(i, j) = -s;
		result.matrix()(j, i) = s;
		result.matrix()(j, j) = c;

		return result;
	}

}

#endif
