#ifndef PASTELMATH_MATRIX_NORM_HPP
#define PASTELMATH_MATRIX_NORM_HPP

#include "pastel/math/matrix_norm.h"

#include <numeric>

namespace Pastel
{

	namespace Norm2_
	{

		template <typename Real, typename NormBijection>
		class AddAxis
		{
		public:
			explicit AddAxis(
				const NormBijection& normBijection)
				: normBijection_(normBijection)
			{
			}

			Real operator()(const Real& left, const Real& right) const
			{
				return normBijection_.addAxis(left, 
					normBijection_.signedAxis(right));
			}

		private:
			const NormBijection& normBijection_;
		};

	}

	template <typename Real, int Height, int Width, 
		typename Expression, typename NormBijection>
	Real norm2(const MatrixExpression<Real, Height, Width, Expression>& matrix,
		const NormBijection& normBijection)
	{
		return std::accumulate(
			matrix.begin(), matrix.end(), (Real)0,
			Norm2_::AddAxis<Real, NormBijection>(normBijection));
		/*
		return std::accumulate(
			matrix.begin(), matrix.end(), (Real)0,
			[](const Real& left, const Real& right) 
		{
			return normBijection.addAxis(left, 
				normBijection.signedAxis(right))
		});
		*/
	}

	template <typename Real, int Height, int Width, typename Expression>
	Real manhattanNorm(
		const MatrixExpression<Real, Height, Width, Expression>& matrix)
	{
		return max(sum(abs(matrix)));
	}

	template <typename Real, int Height, int Width, typename Expression>
	Real maxNorm(
		const MatrixExpression<Real, Height, Width, Expression>& matrix)
	{
		return max(sum(abs(transpose(matrix))));
	}

}

#endif
