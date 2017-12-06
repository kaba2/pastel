// Description: Matrix norm

#ifndef PASTELMATH_MATRIX_NORM_H
#define PASTELMATH_MATRIX_NORM_H

#include "pastel/math/matrix/matrix.h"
#include "pastel/math/matrix/matrix_norm.h"
#include "pastel/math/norm/norm_concept.h"
#include "pastel/math/norm/euclidean_norm.h"

#include <numeric>

namespace Pastel
{

	//! Returns a pointwise matrix norm given by a norm.
	/*!
	This function considers the matrix as a long vector, 
	and computes a vector norm for it. 
	*/
	template <
		typename Real, typename Expression, 
		typename Norm = Euclidean_Norm<Real>,
		Requires<
			Models<Norm, Norm_Concept>
		> = 0>
	Real norm2(
		const MatrixExpression<Real, Expression>& matrix,
		const Norm& norm = Norm())
	{
		integer k = 0;
		auto result = norm();
		for (integer i = 0;i < matrix.height();++i)
		{
			for (integer j = 0;j < matrix.width();++j)
			{
				result.set(k, matrix(i, j));
				++k;
			}
		}
		return ~result;
	}

	//! Returns the induced manhattan matrix norm.
	/*!
	returns:
	max(sum(abs(matrix)))
	*/
	template <typename Real, typename Expression>
	Real manhattanNorm(
		const MatrixExpression<Real, Expression>& matrix)
	{
		return max(sum(abs(matrix)));
	}

	//! Returns the squared Frobenius matrix norm.
	/*!
	returns:
	norm2(matrix)
	*/
	template <typename Real, typename Expression>
	Real frobeniusNorm2(
		const MatrixExpression<Real, Expression>& matrix)
	{
		return norm2(matrix);
	}

	//! Returns the induced maximum matrix norm.
	/*!
	returns:
	max(sum(abs(transpose(matrix))))
	*/
	template <typename Real, typename Expression>
	Real maxNorm(
		const MatrixExpression<Real, Expression>& matrix)
	{
		return max(sum(abs(transpose(matrix))));
	}
}

#endif
