#ifndef PASTELMATH_COORDINATES_HPP
#define PASTELMATH_COORDINATES_HPP

#include "pastel/math/coordinates.h"
#include "pastel/sys/vector_tools.h"

#include "pastel/sys/math_functions.h"

namespace Pastel
{

	template <typename Real, int N>
	Vector<Real, N> cartesianToCylinder(
		const Vector<Real, N>& cartesian, integer k)
	{
		integer dimension = cartesian.n();

		PENSURE_OP(k, >=, 0);
		PENSURE_OP(k, <=, dimension);

		Vector<Real, N> cylinder(ofDimension(dimension));

		// Copy the last (n - k) components as they are.

		for (integer i = k;i < dimension;++i)
		{
			cylinder[i] = cartesian[i];
		}

		// Compute spherical coordinates for the k first
		// components.
		//
		// s_1 = sqrt((x_1)^2 + ... + (x_k)^2)
		// s_i = atan2(sqrt((x_i)^2 + ... + (x_k)^2), x_(i - 1)), for i in ]1, k[.
		// s_k = atan2(x_k, x_(k - 1))
		
		if (k >= 2)
		{
			cylinder[k - 1] = std::atan2(cartesian[k - 1], cartesian[k - 2]);

			Real squareSum = square(cartesian[k - 1]);
			for (integer i = k - 2;i >= 1;--i)
			{
				squareSum += square(cartesian[i]);
				cylinder[i] = std::atan2(std::sqrt(squareSum), cartesian[i - 1]);
			}

			squareSum += square(cartesian[0]);
			cylinder[0] = std::sqrt(squareSum);
		}
		else if (k == 1)
		{
			cylinder[0] = cartesian[0];
		}

		return cylinder;
	}

	template <typename Real, int N>
	Vector<Real, N> cylinderToCartesian(
		const Vector<Real, N>& cylinder, integer k)
	{
		integer dimension = cylinder.n();

		PENSURE_OP(k, >=, 0);
		PENSURE_OP(k, <=, dimension);

		Vector<Real, N> cartesian(ofDimension(dimension));

		// Convert the first k components from spherical
		// coordinates to Cartesian coordinates:
		//
		// x_1 = s_1 cos(s_2)
		// x_i = s_1 sin(s_2) ... sin(s_i) cos(s_(i + 1)), for i in ]1, k[.
		// x_k = s_1 sin(s_2) ... sin(s_(k - 1)) sin(s_(k))

		if (k >= 2)
		{
			Real product = cylinder[0];

			cartesian[0] = product * std::cos(cylinder[1]);
			for (integer i = 1;i < k - 1;++i)
			{
				product *= std::sin(cylinder[i]);
				cartesian[i] = product * std::cos(cylinder[i + 1]);
			}

			cartesian[k - 1] = product * std::sin(cylinder[k - 1]);
		}
		else if (k == 1)
		{
			cartesian[0] = cylinder[0];
		}

		// Copy the last (n - k) components as they are.
		
		for (integer i = k;i < dimension;++i)
		{
			cartesian[i] = cylinder[i];
		}

		return cartesian;
	}

	template <typename Real, int N>
	Vector<Real, N> cartesianToCylinder(
		const Vector<Real, N>& cartesian)
	{
		return Pastel::cartesianToCylinder(
			cartesian, cartesian.n() - 1);
	}

	template <typename Real, int N>
	Vector<Real, N> cylinderToCartesian(
		const Vector<Real, N>& cylinder)
	{
		return Pastel::cylinderToCartesian(cylinder,
			cylinder.n() - 1);
	}

	template <typename Real, int N>
	Vector<Real, N> cartesianToSpherical(
		const Vector<Real, N>& cartesian)
	{
		return cartesianToCylinder(cartesian, 
			cartesian.n());
	}

	template <typename Real, int N>
	Vector<Real, N> sphericalToCartesian(
		const Vector<Real, N>& spherical)
	{
		return cylinderToCartesian(spherical, 
			spherical.n());
	}

}

#endif
