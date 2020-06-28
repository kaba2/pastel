#ifndef PASTELMATH_COORDINATES_DERIVATIVES_HPP
#define PASTELMATH_COORDINATES_DERIVATIVES_HPP

#include "pastel/math/coordinate/coordinates_derivatives.h"

#include "pastel/sys/math_functions.h"
#include "pastel/sys/vector.h"

#include "pastel/math/matrix.h"

namespace Pastel
{

	//! Derivative of the cartesianToSpherical function.
	template <typename Real, int N>
	Matrix<Real, N, N> cartesianToSphericalDerivative(
		const Vector<Real, N>& cartesian)
	{
		integer n = cartesian.size();

		ENSURE_OP(n, >=, 2);

		// This function might seem a bit like a mess.
		// This is because there is much redundancy between
		// the components of the derivative matrix and we
		// want to take advantage of this.
		// However, the point is simply to evaluate into the
		// i:th row the derivative of the cartesianToSpherical
		// function w.r.t. i:th cartesian coordinate.

		Matrix<Real, N, N> result = constantMatrix<Real, N, N>(n, n, 0);

		Real squareSum = square(cartesian[n - 1]) + square(cartesian[n - 2]);

		result(n - 1, n - 1) = cartesian[n - 2] / squareSum;
		result(n - 2, n - 1) = -cartesian[n - 1] / squareSum;

		for (integer i = n - 2;i >= 1;--i)
		{
			Real nextSquareSum = square(cartesian[i - 1]) + squareSum;
			Real sqrtSquareSum = std::sqrt(squareSum);
			Real factor = cartesian[i - 1] /

					(sqrtSquareSum * nextSquareSum);

			result(i - 1, i) = -sqrtSquareSum / nextSquareSum;
			for (integer k = i;k < n;++k)
			{
				result(k, i) = cartesian[k] * factor;
			}

			squareSum = nextSquareSum;
		}

		Real length = std::sqrt(squareSum);
		Real invLength = inverse(length);

		for (integer k = 0;k < n;++k)
		{

			result(k, 0) = cartesian[k] * invLength;
		}

		return result;
	}

	//! Derivative of the sphericalToCartesian function.
	template <typename Real, int N>
	Matrix<Real, N, N> sphericalToCartesianDerivative(
		const Vector<Real, N>& spherical)
	{
		integer n = spherical.size();

		ENSURE_OP(n, >=, 2);

		Matrix<Real, N, N> result(n, n);

		Vector<Real, N> cosSpherical = cos(spherical);
		Vector<Real, N> sinSpherical = sin(spherical);

		{
			Real product = 1;

			result(0, 0) = product * cosSpherical[1];
			for (integer i = 1;i < n - 1;++i)
			{
				product *= sinSpherical[i];
				result(0, i) = product * cosSpherical[i + 1];
			}

			result(0, n - 1) = product * sinSpherical[n - 1];
		}

		for (integer i = 1;i < n;++i)
		{
			Real product = spherical[0];
			for (integer j = 1;j < i;++j)
			{
				product *= sinSpherical[j];
			}
			result(i, i) = -product * sinSpherical[i];

			for (integer j = i + 1;j < n - 2;++j)
			{
				product *= sinSpherical[j];
				result(i, j) = product * cosSpherical[j + 1];
			}
			if (n > 2)
			{
				product *= sinSpherical[n - 2];
			}

			if (i == n - 1)
			{
				result(i, n - 2) = -product * sinSpherical[n - 1];
				result(i, n - 1) = product * cosSpherical[n - 1];
			}
			else
			{
				result(i, n - 2) = product * cosSpherical[n - 1];
				result(i, n - 1) = product * sinSpherical[n - 1];
			}
		}
			
		return result;
	}

	//! Partial derivatives of the sphericalToCartesian function.
	/*!
	Preconditions:
	allGreaterEqual(index, 0)

	Let	f : R^n -> R^n : f(r, alpha_1, ..., alpha_{n - 1})
	where f = sphericalToCartesian.
	
	Using multi-index notation, this function computes df / d(index).
	In other words,	

	d^|index|f / ((dr_0)^index[0] (dalpha_1)^index[1] ... (dalpha_(n-1))^index[n - 1])
	where
	|index| = sum(index)
	*/
	template <typename Real, int N>
	Vector<Real, N> sphericalToCartesianDerivative(
		const Vector<Real, N>& spherical,
		const Vector<integer, N>& index)
	{
		PENSURE(allGreaterEqual(index, 0));
		PENSURE_OP(index.size(), ==, spherical.size());

		integer n = spherical.size();

		if (index[0] >= 2)
		{
			// More than twice differentiated with respect to r.

			return Vector<Real, N>(ofDimension(n), 0);
		}

		Vector<Real, N> result(ofDimension(n), 0);

		// Either r has been differentiated away or not.

		Real x = (index[0] == 0) ? spherical[0] : 1;

		for (integer i = 0;i < n - 1;++i)
		{
			dreal c = std::cos(spherical[i + 1]);
			dreal s = std::sin(spherical[i + 1]);

			// The derivatives of sine are cyclic with
			// a period of 4:
			// sin(x), cos(x), -sin(x), -cos(x), sin(x), ...
			// Thus we can reduce the cases to those
			// of 0, 1, 2, or 3 times differentiated.

			const integer timesMod4 = index[i + 1] & 3;

			switch(timesMod4)
			{
			case 0:
				// No differentiation.
				result[i] = x * c;
				x *= s;
				break;
			case 1:
				// Once differentiated.
				result[i] = x * -s;
				x *= c;
				break;
			case 2:
				// Twice differentiated.
				result[i] = x * -c;
				x *= -s;
				break;
			case 3:
				// Thrice differentiated.
				result[i] = x * s;
				x *= -c;
				break;
			};
		}

		result[n - 1] = x;

		return result;
	}

}

#endif
