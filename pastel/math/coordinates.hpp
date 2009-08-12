#ifndef PASTEL_COORDINATES_HPP
#define PASTEL_COORDINATES_HPP

#include "pastel/math/coordinates.h"
#include "pastel/sys/vector_tools.h"

#include "pastel/sys/math_functions.h"

namespace Pastel
{

	template <int N, typename Real>
	Vector<Real, N> cartesianToCylinder(
		const Vector<Real, N>& cartesian, integer k)
	{
		const integer dimension = cartesian.dimension();

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
		// s_i = atan(sqrt((x_i)^2 + ... + (x_k)^2) / x_(i - 1)), for i in ]1, k[.
		// s_k = atan(x_k / x_(k - 1))

		cylinder[k - 1] = std::atan(cartesian[k - 1] / cartesian[k - 2]);

		Real squareSum = square(cartesian[k - 1]);
		for (integer i = k - 2;i >= 1;--i)
		{
			squareSum += square(cartesian[i]);
			cylinder[i] = std::atan(std::sqrt(squareSum) / cartesian[i - 1]);
		}

		squareSum += square(cartesian[0]);
		cylinder[0] = std::sqrt(squareSum);

		return cylinder;
	}

	template <int N, typename Real>
	Vector<Real, N> cylinderToCartesian(
		const Vector<Real, N>& cylinder, integer k)
	{
		const integer dimension = cylinder.dimension();

		PENSURE_OP(k, >=, 0);
		PENSURE_OP(k, <=, dimension);

		Vector<Real, N> cartesian(ofDimension(dimension));

		// Convert the first k components from spherical
		// coordinates to Cartesian coordinates:
		//
		// x_1 = s_1 cos(s_2)
		// x_i = s_1 sin(s_2) ... sin(s_i) cos(s_(i + 1)), for i in ]1, k[.
		// x_k = s_1 sin(s_2) ... sin(s_(k - 1)) sin(s_(k))

		Real product = cylinder[0];
		cartesian[0] = product * std::cos(cylinder[1]);
		for (integer i = 1;i < k - 1;++i)
		{
			product *= std::sin(cylinder[i]);
			cartesian[i] = product * std::cos(cylinder[i + 1]);
		}

		cartesian[k - 1] = product * std::sin(cylinder[k - 1]);

		// Copy the last (n - k) components as they are.
		
		for (integer i = k;i < dimension;++i)
		{
			cartesian[i] = cylinder[i];
		}

		return cartesian;
	}

	template <int N, typename Real>
	Vector<Real, N> cartesianToCylinder(
		const Vector<Real, N>& cartesian)
	{
		return Pastel::cartesianToCylinder(
			cartesian, cartesian.dimension() - 1);
	}

	template <int N, typename Real>
	Vector<Real, N> cylinderToCartesian(
		const Vector<Real, N>& cylinder)
	{
		return Pastel::cylinderToCartesian(cylinder,
			cylinder.dimension() - 1);
	}

	template <int N, typename Real>
	Vector<Real, N> cartesianToSpherical(
		const Vector<Real, N>& cartesian)
	{
		return cartesianToCylinder(cartesian, 
			cartesian.dimension());
	}

	template <int N, typename Real>
	Vector<Real, N> sphericalToCartesian(
		const Vector<Real, N>& spherical)
	{
		return cylinderToCartesian(spherical, 
			spherical.dimension());
	}

}

#endif
