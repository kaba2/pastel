#ifndef PASTEL_COORDINATES_DERIVATIVES_HPP
#define PASTEL_COORDINATES_DERIVATIVES_HPP

#include "pastel/math/coordinates_derivatives.h"

#include "pastel/sys/math_functions.h"

namespace Pastel
{

	template <typename Real, int N>
	Matrix<Real, N, N> cartesianToSphericalDerivative(
		const Vector<Real, N>& cartesian)
	{
		const integer n = cartesian.size();

		ENSURE_OP(n, >=, 2);

		// This function might seem a bit like a mess.
		// This is because there is much redundancy between
		// the components of the derivative matrix and we
		// want to take advantage of this.
		// However, the point is simply to evaluate into the
		// i:th row the derivative of the cartesianToSpherical
		// function w.r.t. i:th cartesian coordinate.

		Matrix<Real, N, N> result(n, n, 0);

		Real squareSum = square(cartesian[n - 1]) + square(cartesian[n - 2]);

		result(n - 1, n - 1) = cartesian[n - 2] / squareSum;
		result(n - 2, n - 1) = -cartesian[n - 1] / squareSum;

		for (integer i = n - 2;i >= 1;--i)
		{
			const Real nextSquareSum = square(cartesian[i - 1]) + squareSum;
			const Real sqrtSquareSum = std::sqrt(squareSum);
			const Real factor = cartesian[i - 1]) /
					(sqrtSquareSum * nextSquareSum);

			result(i - 1, i) = -sqrtSquareSum / nextSquareSum;
			for (integer k = i;k < n;++k)
			{
				result(k, i) = cartesian[k] * factor;
			}

			squareSum = nextSquareSum;
		}

		const Real length = std::sqrt(squareSum);
		const Real invLength = inverse(length);

		for (integer k = 0;k < n;++k)
		{
			result(k, 0) = cartesian[k] * invLength;
		}

		return result;
	}

	template <typename Real, int N>
	Matrix<Real, N, N> sphericalToCartesianDerivative(
		const Vector<Real, N>& cartesian)
	{
		const integer n = cartesian.size();

		ENSURE_OP(n, >=, 2);

		Matrix<Real, N, N> result(n, n, 0);
	}

}

#endif
