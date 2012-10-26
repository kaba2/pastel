#ifndef PASTELMATH_RANDOM_ROTATION_MATRIX_HPP
#define PASTELMATH_RANDOM_ROTATION_MATRIX_HPP

#include "pastel/math/random_rotation_matrix.h"
#include "pastel/math/random_orthogonal_matrix.h"
#include "pastel/math/matrix_determinant.h"

namespace Pastel
{

	template <typename Real>
	void setRandomRotation(
		Matrix<Real>& q)
	{
		setRandomOrthogonal(q);
		if (determinant(q) < 0)
		{
			for (integer i = 0;i < q.m();++i)
			{
				q(i, 0) = -q(i, 0);
			}
		}
	}

	template <typename Real>
	Matrix<Real> randomRotationMatrix(
		integer n)
	{
		ENSURE_OP(n, >=, 0);
		
		Matrix<Real> q(n);
		setRandomRotation(q);
		return q;
	}

}

#endif
