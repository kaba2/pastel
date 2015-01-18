#ifndef PASTELMATH_MATRIX_NORM_HPP
#define PASTELMATH_MATRIX_NORM_HPP

#include "pastel/math/matrix/matrix_norm.h"
#include "pastel/math/normbijection/euclidean_normbijection.h"

#include <numeric>

namespace Pastel
{

	template <typename Real, typename Expression, 
		typename NormBijection>
	Real norm2(
		const MatrixExpression<Real, Expression>& matrix,
		const NormBijection& normBijection)
	{
		Real result = 0;
		for (integer i = 0;i < matrix.height();++i)
		{
			for (integer j = 0;j < matrix.width();++j)
			{
				result = normBijection.addAxis(result, 
					normBijection.signedAxis(matrix(i, j)));
			}
		}

		return result;
	}

	template <typename Real, typename Expression>
	Real manhattanNorm(
		const MatrixExpression<Real, Expression>& matrix)
	{
		return max(sum(abs(matrix)));
	}

	template <typename Real, typename Expression>
	Real frobeniusNorm2(
		const MatrixExpression<Real, Expression>& matrix)
	{
		return norm2(matrix, Euclidean_NormBijection<Real>());
	}

	template <typename Real, typename Expression>
	Real frobeniusNorm(
		const MatrixExpression<Real, Expression>& matrix)
	{
		return std::sqrt(frobeniusNorm2(matrix));
	}

	template <typename Real, typename Expression>
	Real maxNorm(
		const MatrixExpression<Real, Expression>& matrix)
	{
		return max(sum(abs(transpose(matrix))));
	}

}

#endif
