#ifndef PASTELMATH_RANDOM_SPD_MATRIX_HPP
#define PASTELMATH_RANDOM_SPD_MATRIX_HPP

#include "pastel/math/random_spd_matrix.h"
#include "pastel/math/random_orthogonal_matrix.h"

#include "pastel/sys/random_uniform.h"

#include <algorithm>
#include <vector>

namespace Pastel
{

	template <typename Real>
	void setRandomSymmetricPositiveDefinite(
		const PASTEL_NO_DEDUCTION(Real)& determinant,
		Matrix<Real>& result)
	{
		integer n = result.width();

		ENSURE_OP(result.width(), ==, result.height());

		// Generate a random partition of the
		// interval [0, -ln(d)] to n subintervals
		// represented by (n + 1) partition points.

		Real xDelta = -std::log(determinant);

		std::vector<Real> partitionSet;
		partitionSet.reserve(n + 1);
		partitionSet.push_back(0);
		partitionSet.push_back(xDelta);
		
		for (integer i = 0;i < n - 1;++i)
		{

			partitionSet.push_back(random<Real>() * xDelta);
		}

		std::sort(partitionSet.begin(), partitionSet.end());

		// Generate a random rotation matrix.
		setRandomOrthogonal(result);

		// Multiply the columns of the rotation matrix
		// with square root of the diagonal element of D.
		for (integer j = 0;j < n;++j)
		{
			Real b = partitionSet[j + 1] - partitionSet[j];
			for (integer i = 0;i < n;++i)
			{

				result(i, j) *= std::exp(-b / 2);
			}
		}

		result *= transpose(result);
	}

	template <typename Real>
	void setRandomSymmetricPositiveDefinite(
		const PASTEL_NO_DEDUCTION(Real)& determinant,
		const PASTEL_NO_DEDUCTION(Real)& condition,
		Matrix<Real>& result)
	{
		ENSURE_OP(result.width(), ==, result.height());
		ENSURE_OP(condition, >=, 1);
		ENSURE_OP(determinant, >, 0);

		integer n = result.width();

		ENSURE_OP(n, >, 1);

		Real a = 

			((n - 1) * std::log(condition) - 
			std::log(determinant)) / n;
		
		Real b =
			a - std::log(condition);

		// Generate a random rotation matrix.
		setRandomRotation(result);

		// Multiply the columns of the rotation matrix
		// with square roots of the diagonal elements of D.
		for (integer i = 0;i < n;++i)
		{

			result(i, 0) *= std::exp(-a / 2);
		}

		for (integer j = 1;j < n;++j)
		{
			for (integer i = 0;i < n;++i)
			{
				result(i, j) *= std::exp(-b / 2);
			}
		}

		result *= transpose(result);
	}

}

#endif
