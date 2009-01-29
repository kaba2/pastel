#ifndef PASTELMATH_COORDINATES_HPP
#define PASTELMATH_COORDINATES_HPP

#include "pastel/math/coordinates.h"
#include "pastel/sys/vector_tools.h"

#include "pastel/sys/mathfunctions.h"

namespace Pastel
{

	template <typename Real>
	Vector<3, Real> cartesianToCylinder(
		const Vector<3, Real>& cartesian)
	{
		const Real radius(std::sqrt(cartesian[0] * cartesian[0] +
			cartesian[1] * cartesian[1]));
		const Real angle(positiveRadians<Real>(std::atan2(cartesian[1], cartesian[0])));

		return Vector<3, Real>(radius, angle, cartesian[2]);
	}

	template <typename Real>
	Vector<3, Real> cylinderToCartesian(
		const Vector<3, Real>& cylinder)
	{
		return Vector<3, Real>(
			cylinder[0] * std::cos(cylinder[1]),
			cylinder[0] * std::sin(cylinder[1]),
			cylinder[2]);
	}

	template <int N, typename Real>
	typename boost::enable_if_c<N == 1, Vector<N, Real> >::type
		cartesianToSpherical(
		const Vector<N, Real>& cartesian)
	{
		return cartesian;
	}

	template <int N, typename Real>
	typename boost::enable_if_c<(N > 1), Vector<N, Real> >::type
		cartesianToSpherical(
		const Vector<N, Real>& cartesian)
	{
		// tan(alpha_(n - 1)) = x_n / x_(n - 1)
		// tan(alpha_(n - 2)) = sqrt((x_n)^2 + (x_(n - 1))^2) / x_(n - 2)
		// ...
		// tan(alpha_(n - k)) = sqrt((x_n)^2 + ... + (x_(n - k + 1))^2) / x_(n - k)
		// ...
		// tan(alpha_1) = sqrt((x_n)^2 + ... + (x_2)^2) / x_1

		Vector<N, Real> result;
		result[N - 1] = std::atan(cartesian[N - 1] / cartesian[N - 2]);

		Real squareSum = cartesian[N - 1] * cartesian[N - 1];

		for (integer i = N - 2;i >= 1;--i)
		{
			squareSum += cartesian[i] * cartesian[i];
			result[i] = std::atan(std::sqrt(squareSum) / cartesian[i - 1]);
		}

		squareSum += cartesian[0] * cartesian[0];
		result[0] = std::sqrt(squareSum);

		return result;
	}

	/*
	template <typename Real>
	Vector<3, Real> cartesianToSpherical(
		const Vector<3, Real>& cartesian)
	{
		const Real radius(norm(cartesian));
		const Real theta(positiveRadians<Real>(std::atan2(cartesian[1], cartesian[0])));
		const Real phi(std::acos(cartesian[2] / radius));

		return Vector<3, Real>(radius, theta, phi);
	}
	*/

	template <int N, typename Real>
	typename boost::enable_if_c<N == 1, Vector<N, Real> >::type
		sphericalToCartesian(
		const Vector<N, Real>& spherical)
	{
		return spherical;
	}

	template <int N, typename Real>
	typename boost::enable_if_c<(N > 1), Vector<N, Real> >::type
		sphericalToCartesian(
		const Vector<N, Real>& spherical)
	{
		// x_1 = r cos(alpha_1)
		// x_2 = r sin(alpha_1) cos(alpha_2)
		// ...
		// x_k = r sin(alpha_1) ... sin(alpha_(k - 1)) cos(alpha_k)
		// ...
		// x_n = r sin(alpha_1) ... sin(alpha_(n - 1)) sin(alpha_n)

		Vector<N, Real> cartesian;
		Real product = spherical[0];

		cartesian[0] = product * std::cos(spherical[1]);
		for (integer i = 1;i < N - 1;++i)
		{
			product *= std::sin(spherical[i]);
			cartesian[i] = product * std::cos(spherical[i + 1]);
		}

		cartesian[N - 1] = product * std::sin(spherical[N - 1]);

		return cartesian;
	}

	/*
	template <typename Real>
	Vector<3, Real> sphericalToCartesian(
		const Vector<3, Real>& spherical)
	{
		const Real radius(spherical[0]);
		const Real cosTheta(std::cos(spherical[1]));
		const Real sinTheta(std::sin(spherical[1]));
		const Real cosPhi(std::cos(spherical[2]));
		const Real sinPhi(std::sin(spherical[2]));

		return Vector<3, Real>(
			radius * cosTheta * sinPhi,
			radius * sinTheta * sinPhi,
			radius * cosPhi);
	}
	*/

	template <int N, typename Real>
	typename boost::enable_if_c<N == 2, Vector<1, Real> >::type
		cartesianToDirection(
		const Vector<N, Real>& cartesian)
	{
		return Vector<1, Real>(std::atan(cartesian[1] / cartesian[0]));
	}

	template <int N, typename Real>
	typename boost::enable_if_c<(N > 2), Vector<N - 1, Real> >::type
		cartesianToDirection(
		const Vector<N, Real>& cartesian)
	{
		// tan(alpha_(n - 1)) = x_n / x_(n - 1)
		// tan(alpha_(n - 2)) = sqrt((x_n)^2 + (x_(n - 1))^2) / x_(n - 2)
		// ...
		// tan(alpha_(n - k)) = sqrt((x_n)^2 + ... + (x_(n - k + 1))^2) / x_(n - k)
		// ...
		// tan(alpha_1) = sqrt((x_n)^2 + ... + (x_2)^2) / x_1

		Vector<N - 1, Real> direction;
		direction[N - 2] = std::atan(cartesian[N - 1] / cartesian[N - 2]);

		Real squareSum = cartesian[N - 1] * cartesian[N - 1];

		for (integer i = N - 2;i >= 1;--i)
		{
			squareSum += cartesian[i] * cartesian[i];
			direction[i - 1] = std::atan(std::sqrt(squareSum) / cartesian[i - 1]);
		}

		return direction;
	}

	/*
	template <typename Real>
	Real cartesianToDirection(
		const Vector<2, Real>& cartesian)
	{
		return positiveRadians<Real>(std::atan2(cartesian[1], cartesian[2]));
	}
	*/

	template <typename Real>
	Vector<2, Real> cartesianToDirection(
		const Vector<3, Real>& cartesian)
	{
		const Real radius(norm(cartesian));
		const Real theta(positiveRadians<Real>(std::atan2(cartesian[1], cartesian[0])));
		const Real phi(std::acos(cartesian[2] / radius));

		return Vector<2, Real>(theta, phi);
	}

	template <typename Real>
	Vector<2, Real> directionToCartesian(
		const Vector<1, Real>& direction)
	{
		return Vector<2, Real>(
			std::cos(angle[0]),
			std::sin(angle[0]));
	}

	template <typename Real>
	Vector<3, Real> directionToCartesian(
		const Vector<2, Real>& direction)
	{
		const Real cosTheta(std::cos(direction[0]));
		const Real sinTheta(std::sin(direction[0]));
		const Real cosPhi(std::cos(direction[1]));
		const Real sinPhi(std::sin(direction[1]));

		return Vector<3, Real>(
			cosTheta * sinPhi,
			sinTheta * sinPhi,
			cosPhi);
	}

}

#endif
