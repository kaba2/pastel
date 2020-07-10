// Description: Conversion between coordinate systems
// Detail: Cartesian, cylinder, and spherical coordinate systems.

#ifndef PASTELMATH_COORDINATES_H
#define PASTELMATH_COORDINATES_H

#include "pastel/sys/mytypes.h"
#include "pastel/sys/vector.h"
#include "pastel/sys/math_functions.h"

#include "pastel/math/coordinate/coordinates.h"

namespace Pastel
{

	//! Converts cartesian coordinates to generalized cylinder coordinates.
	/*!
	Preconditions:
	k >= 0
	k <= cartesian.n()

	The generalized cylinder coordinates are computed as follows.
	Given a Cartesian coordinate vector	(x, y) in R^n, with 
	x in R^k, and z in R^(n - k), the generalized cylinder coordinates 
	are given by (s, y), where s = spherical coordinates of x:
	
	s_1 = sqrt((x_1)^2 + ... + (x_k)^2)
	s_i = atan(sqrt((x_i)^2 + ... + (x_k)^2) / x_(i - 1)), for i in ]1, k[.
	s_k = atan(x_k / x_(k - 1))
	*/
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

	//! Converts generalized cylinder coordinates to cartesian coordinates.
	/*!
	Preconditions:
	k >= 0
	k <= cylinder.n()

	Given generalized cylinder coordinates (s, y) in R^n
	(s in R^k), the Cartesian coordinates (x, y) in R^n
	(x in R^k) are computed by converting s from spherical 
	coordinates to Cartesian coordinates:

	x_1 = s_1 cos(s_2)
	x_i = s_1 sin(s_2) ... sin(s_i) cos(s_(i + 1)), for i in ]1, k[.
	x_k = s_1 sin(s_2) ... sin(s_(k - 1)) sin(s_(k))
	*/
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

	//! Converts cartesian coordinates to cylinder coordinates.
	/*!
	This is a convenience function that calls
	cartesianToCylinder(cartesian, dimension - 1).
	*/
	template <typename Real, int N>
	Vector<Real, N> cartesianToCylinder(
		const Vector<Real, N>& cartesian)
	{
		return Pastel::cartesianToCylinder(
			cartesian, cartesian.n() - 1);
	}

	//! Converts cylinder coordinates to cartesian coordinates.
	/*!
	This is a convenience function that calls
	cylinderToCartesian(cartesian, dimension - 1).
	*/
	template <typename Real, int N>
	Vector<Real, N> cylinderToCartesian(
		const Vector<Real, N>& cylinder)
	{
		return Pastel::cylinderToCartesian(cylinder,
			cylinder.n() - 1);
	}

	//! Converts cartesian coordinates to spherical coordinates.
	/*!
	This is a convenience function that calls
	cartesianToCylinder(cartesian, dimension).
	*/
	template <typename Real, int N>
	Vector<Real, N> cartesianToSpherical(
		const Vector<Real, N>& cartesian)
	{
		return cartesianToCylinder(cartesian, 
			cartesian.n());
	}

	//! Converts spherical coordinates to cartesian coordinates.
	/*!
	This is a convenience function that calls
	cylinderToCartesian(cartesian, dimension).
	*/
	template <typename Real, int N>
	Vector<Real, N> sphericalToCartesian(
		const Vector<Real, N>& spherical)
	{
		return cylinderToCartesian(spherical, 
			spherical.n());
	}

}

#endif
