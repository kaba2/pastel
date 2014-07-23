#ifndef PASTELMATH_UNIFORM_DISTORTION_HPP
#define PASTELMATH_UNIFORM_DISTORTION_HPP

#include "pastel/math/uniform_distortion.h"
#include "pastel/math/coordinates.h"

#include "pastel/sys/math_functions.h"
#include "pastel/sys/constants.h"
#include "pastel/sys/vector_tools.h"
#include "pastel/sys/random.h"

namespace Pastel
{

	template <typename Real, int N>
	PASTEL_ENABLE_IF_C(N == 1, (Vector<Real, N>))
		uniformlySampleAnnulus(
		const Vector<Real, N>& uv,
		const PASTEL_NO_DEDUCTION(Real)& minRadius,
		const PASTEL_NO_DEDUCTION(Real)& maxRadius)
	{
		PENSURE_OP(minRadius, >=, 0);
		PENSURE_OP(maxRadius, >=, 0);
		PENSURE_OP(minRadius, <, maxRadius);

		// See the implementation for the 3d version
		// for derivation.

		if (uv[0] < 0.5)
		{
			return linear(
				-minRadius,
				-maxRadius,
				2 * uv[0]);
		}

		return linear(
			minRadius,
			maxRadius,
			2 * uv[0] - 1);
	}

	template <typename Real, int N>
	PASTEL_ENABLE_IF_C(N == 2, (Vector<Real, N>))
		uniformlySampleAnnulus(
		const Vector<Real, N>& uv,
		const PASTEL_NO_DEDUCTION(Real)& minRadius,
		const PASTEL_NO_DEDUCTION(Real)& maxRadius)
	{
		PENSURE_OP(minRadius, >=, 0);
		PENSURE_OP(maxRadius, >=, 0);
		PENSURE_OP(minRadius, <, maxRadius);

		// See the implementation for the 3d version
		// for derivation.

		Real r =
			std::sqrt(
			linear(

			minRadius * minRadius,
			maxRadius * maxRadius,
			uv[1]));
		Real theta =

			2 * constantPi<Real>() * uv[0];

		return sphericalToCartesian(
			Vector<Real, 2>(r, theta));
	}

	template <typename Real, int N>
	PASTEL_ENABLE_IF_C(N == 3, (Vector<Real, N>))
		uniformlySampleAnnulus(
		const Vector<Real, N>& uv,
		const PASTEL_NO_DEDUCTION(Real)& minRadius,
		const PASTEL_NO_DEDUCTION(Real)& maxRadius)
	{
		PENSURE_OP(minRadius, >=, 0);
		PENSURE_OP(maxRadius, >=, 0);
		PENSURE_OP(minRadius, <, maxRadius);

		// Uniform sampling of a hyperannulus
		// ==================================
		//
		// Problem
		// -------
		//
		// Given is a uniformly distributed random variable
		// in [0, 1]^N. Distort the random variable such
		// that it becomes uniformly distributed
		// in the (r_min, r_max)-hyperannulus.
		//
		// Solution
		// --------
		//
		// We will first outline the strategy. The points
		// of the hyperannulus are best described
		// via hyperspherical coordinates. Thus our
		// strategy will be to find a transform
		// from the [0, 1]^N to the hyperspherical coordinates
		// with the range [r_min, r_max] x [0, pi]^(N - 2) x [0, 2pi].
		// The corresponding cartesian coordinates are then get via
		// coordinate conversion.
		//
		// Here are some important definite integrals that
		// we will be needing:
		//
		// int[0..X] sin^0(x) dx
		// = [0..X] 1
		// = X
		//
		// int[0..X] sin^1(x) dx
		// = [0..X] -cos(x)
		// = 1 - cos(X)
		//
		// int[0..X] sin^2(x) dx
		// = int[0..X] 1/2 + 1/2 cos(2x) dx
		// = (1/2)X + (1/4) int[0..X] 2 cos(2x) dx
		// = (1/2)X + (1/4) [0..X] sin(2x)
		// = (1/2)X + (1/4) sin(2X)
		//
		// ...
		//
		// int[0..X] sin^10(x) dx
		// = -(193/256) sin(X) cos(X) + (149/128) sin(X) cos(X)^3 - (171/160) sin(X) cos(X)^5
		// + (41/80) sin(X) cos(X)^7 - (1/10) sin(X) cos(X)^9 + (63/256) X
		//
		// We begin by computing the partial volume V(R, A_1, A_2, ..., A_(N - 1)) of the hyperannulus,
		// where (R, A_1, ..., A_(N - 1)) is a hyperspherical coordinate.
		// Substituting R = r_max, A_1 = pi, A_2 = pi, ..., A_(N - 2) = pi, A_(N - 1) = 2 pi
		// will give the volume of the hyperannulus. Ultimately, our main problem is to find
		// out how the uniform random variable u_k (and u_r) must be transformed to the A_k (and R)
		// such that the volume changes linearly with u_k (and u_r).
		//
		// The partial volume is given by integrating the hyperspherical volume element:
		//
		// V(R, A_1, A_2, ..., A_(N - 1))
		// =
		// int[r = r_min..R] r^(N - 1) *
		// int[a_1 = 0..A_1] sin^(N - 2)(a_1) da_1 *
		// int[a_2 = 0..A_2] sin^(N - 3)(a_2) da_2 *
		// ...
		// int[a_(N - 2) = 0..A_(N - 2)] sin(a_(N - 2)) da_(N - 2) *
		// int[a_(N - 1) = 0..A_(N - 1)] 1 da_(N - 1)
		// =
		// (A_(N - 1) / N)(R^N - r_min^N) *
		// int[a_1 = 0..A_1] sin^(N - 2)(a_1) da_1 *
		// int[a_2 = 0..A_2] sin^(N - 3)(a_2) da_2 *
		// ...
		// int[a_(N - 2) = 0..A_(N - 2)] sin(a_(N - 2)) da_(N - 2)
		//
		// Unfortunately computing V for arbitrary dimensions
		// is hard. You can get the answer in nice form for any specific dimension,
		// however, there seems to be no structure to aid in the construction
		// of the expression: just look at the integral for the sin^10(x) above.
		// However, before rushing to solve this problem, note that there
		// will be another more serious problem with higher dimensions (which
		// can not be solved). Let us compute V for dimensions 1, 2, and 3:
		//
		// V(R)
		// = 2 (R - r_min)
		//
		// V(R, A_1)
		// = (1/2) (R^2 - r_min^2) A_1
		//
		// =>
		// V(r_max, 2pi)
		// = pi (r_max^2 - r_min^2)
		//
		// V(R, A_1, A_2)
		// = (1/3) (R^3 - r_min^3) (1 - cos(A_1)) A_2
		//
		// =>
		// V(r_max, pi, 2pi)
		// = (4pi/3) (r_max^3 - r_min^3)
		//
		// Encouraged by the success in these dimensions, we shall
		// try the 4-dimensional case:
		//
		// V(R, A_1, A_2, A_3)
		// = (1/4) (R^4 - r_min^4) ((1/2)A_1 + (1/4)sin(2 A_1)) (1 - cos(A_2)) A_3
		//
		// We again get a closed form solution. Actually, we can get a closed form
		// solution for any dimension, although with substantial trouble.
		// However, note that in this 4d case A_1 is not solvable in closed form from the equation.
		// Because higher dimensional formulas contain a similar term (and yet more complex
		// terms), it will be the case that with higher dimensions some parameter is
		// not solvable in closed form. We then conclude that
		// this technique is not applicable to dimensions higher than 3.
		//
		// Let us now proceed with the dimensions 1, 2, and 3.
		// The technique is to substitute all parameters in V
		// with their maxima except for one parameter.
		// The idea is to find out how that
		// single parameter must be varied in order for the partial
		// volume to change linearly. This will gives us the
		// perturbation from the hypercube to the hyperannulus.
		//
		// 2d:
		//
		// u_r V(r_max, 2pi) = V(R, 2pi) = pi (R^2 - r_min^2)
		// =>
		// u_r pi (r_max^2 - r_min^2) = pi (R^2 - r_min^2)
		// =>
		// u_r (r_max^2 - r_min^2) = (R^2 - r_min^2)
		// =>
		// R = sqrt(u_r (r_max^2 - r_min^2) + r_min^2)
		//
		// u_1 V(r_max, 2pi) = V(r_max, A_1) = (1/2) (r_max^2 - r_min^2) A_1
		// =>
		// u_1 pi (r_max^2 - r_min^2) = (1/2) (r_max^2 - r_min^2) A_1
		// =>
		// u_1 pi = (1/2) A_1
		// =>
		// A_1 = 2 pi u_1
		//
		// 3d:
		//
		// u_r V(r_max, pi, 2pi) = V(R, pi, 2pi) = (4pi/3) (R^3 - r_min^3)
		// =>
		// u_r (4pi/3) (r_max^3 - r_min^3) = (4pi/3) (R^3 - r_min^3)
		// =>
		// u_r (r_max^3 - r_min^3) = (R^3 - r_min^3)
		// =>
		// R = (u_r (r_max^3 - r_min^3) + r_min^3)^(1/3)
		//
		// u_1 V(r_max, pi, 2pi) = V(r_max, A_1, 2pi) = (2pi/3) (r_max^3 - r_min^3) (1 - cos(A_1))
		// =>
		// u_1 (4pi/3) (r_max^3 - r_min^3) = (2pi/3) (r_max^3 - r_min^3) (1 - cos(A_1))
		// =>
		// 2 u_1 = 1 - cos(A_1)
		// =>
		// cos(A_1) = 1 - 2 u_1
		// =>
		// A_1 = acos(1 - 2 u_1)
		//
		// u_2 V(r_max, pi, 2pi) = V(r_max, pi, A_2) = (2/3) (r_max^3 - r_min^3) A_2
		// =>
		// u_2 (4pi/3) (r_max^3 - r_min^3) = (2/3) (r_max^3 - r_min^3) A_2
		// =>
		// A_2 = 2 pi u_2
		//
		// Summary
		// -------
		//
		// To distort a uniform distribution in [0, 1]^N to a
		// uniform distribution in a (r_min, r_max)-hyperannulus described
		// by hyperspherical coordinates, use the following
		// formulas:
		//
		// 2d:
		//
		// Let [u_r, u_1] in [0, 1]^2.
		//
		// R = sqrt(u_r (r_max^2 - r_min^2) + r_min^2)
		// A_1 = 2 pi u_1
		//
		// 3d:
		//
		// Let [u_r, u_1, u_2] in [0, 1]^3.
		//
		// R = (u_r (r_max^3 - r_min^3) + r_min^3)^(1/3)
		// A_1 = acos(1 - 2 u_1)
		// A_2 = 2 pi u_2
		//
		// Higher dimensions:
		//
		// There is no closed form solution.

		Real r =
			std::pow(
			linear(

			minRadius * minRadius * minRadius,
			maxRadius * maxRadius * maxRadius,
			uv[0]),
			(Real)1/3);
		Real phi =

			std::acos(1 - 2 * uv[1]);
		Real theta =

			2 * constantPi<Real>() * uv[2];

		return sphericalToCartesian(
			Vector<Real, 3>(r, phi, theta));
	}

	// uniformlySampleSimplex

	template <typename Real, int N>
	PASTEL_ENABLE_IF_C(N == 1, (Vector<Real, N>))
		uniformlySampleSimplex(
		const Vector<Real, N>& uv)
	{
		return uv;
	}

	template <typename Real, int N>
	PASTEL_ENABLE_IF_C(N == 2, (Vector<Real, N>))
		uniformlySampleSimplex(
		const Vector<Real, N>& uv)
	{
		// Let p be a uniform probability distribution
		// in the unit n-simplex
		// {x | sum(x) <= 1, allGreaterEqual(x, 0)}
		// The volume of the simplex is 1/n!.
		// Thus:
		// p(x, y) = n!
		//
		// The marginal density w.r.t. to y is:
		// p_y(x) = int[0..1 - x] p(x, y) dy
		// = 2 * (1 - x)
		// Its cumulative probability is:
		// P_y(X) = int[0..X] p_y(x) dx
		// = 2 * (X - X^2 / 2) = 2 * X - X^2
		//
		// Invert the cumulative probability function to get:
		// -X^2 + 2 * X - u = 0
		// =>
		// X = [-2 + sqrt(4 - 4 * u)] / -2
		// = 1 - sqrt(1 - u)
		//
		// (1 - u) is also uniformly distributed, thus we
		// can choose:
		// X = 1 - sqrt(u)
		//
		// Conditional density w.r.t to x is:
		// p(y | x) = p(x, y) / p_y(x)
		// = 2 / (2 * (1 - x)) = 1 / (1 - x)
		//
		// Its cumulative probability is:
		// P(y | x) = int[0.. Y] p(y | x) dy
		// = Y / (1 - x)
		//
		// Invert the cumulative probability function to get:
		// => Y = (1 - X) * v
		// = sqrt(u) * v

		Real uSqrt(std::sqrt(uv[0]));
		return Vector<Real, N>(

			1 - uSqrt, uSqrt * uv[1]);
	}

	template <typename Real, int N>
	PASTEL_ENABLE_IF_C(N >= 3 || N == Dynamic, (Vector<Real, N>))
		uniformlySampleSimplex(
		const Vector<Real, N>& uv)
	{
		integer n = uv.n();

		std::vector<Real> partition;
		partition.reserve(n + 2);
		partition.push_back(0);
		for (integer i = 0;i < n;++i)
		{
			partition.push_back(uv[i]);
		}
		partition.push_back(1);

		std::sort(partition.begin(), partition.end());

		Vector<Real, N> simplex(ofDimension(n));

		for (integer i = 0;i < n;++i)
		{
			simplex[i] = partition[i + 1] - partition[i];
		}

		return simplex;
	}

	// uniformlySampleBall

	template <typename Real, int N>
	PASTEL_ENABLE_IF_C(N == 1, (Vector<Real, N>))
		uniformlySampleBall(

		const Vector<Real, N>& uv)
	{
		return 2 * uv - 1;
	}

	template <typename Real, int N>
	PASTEL_ENABLE_IF_C(N == 2, (Vector<Real, N>))
		uniformlySampleBall(
		const Vector<Real, N>& uv)
	{
		// "A Low Distortion Map Between Disk and Square",
		// Peter Shirley, Kenneth Chiu, 1997

		// A [-1, 1]^2 box is divided into four quadrants
		// as follows:
		//  ____
		// |\  /|
		// | \/ |
		// | /\ |
		// |/__\|
		//
		// This box is distorted to a unit disk
		// such that each quadrant has its own transformation.
		//
		// Take the right triangle as an example.
		// The result will be given in polar coordinates.
		// Lines aligned with x-axis are mapped to radial
		// lines. Lines aligned with y-axis are mapped to
		// circle arcs.
		//
		// In short, concentric boxes are
		// mapped to concentric circles.

		const Vector<Real, 2> st(2 * uv - 1);

		Real radius(0);
		Real scaledTheta(0);

		if (st.x() > st.y())
		{
			if (st.x() > -st.y())
			{
				// Right triangle
				radius = st.x();
				scaledTheta = st.y() / radius;
			}
			else
			{
				// Bottom triangle
				radius = -st.y();
				scaledTheta = 6 + st.x() / radius;
			}
		}
		else
		{
			if (st.x() > -st.y())
			{
				// Top triangle
				radius = st.y();
				scaledTheta = 2 - st.x() / radius;
			}
			else
			{
				// Left triangle
				radius = -st.x();
				scaledTheta = 4 - st.y() / radius;
			}
		}

		Real theta(

			scaledTheta * (constantPi<Real>() / 4));

		return Vector<Real, 2>(
			radius * std::cos(theta),
			radius * std::sin(theta));
	}

	/*
	template <typename Real, int N>
	PASTEL_ENABLE_IF_C(N == 2, (Vector<Real, N>))
		uniformlySampleBall(
		const Vector<Real, N>& uv)
	{
		// This is the old implementation of 2-dimensional
		// uniform sampling from a ball.
		// Sampling uniformly from a ball is a special case of
		// sampling from an annulus: the unit-ball is a
		// (0, 1)-annulus. See the documentation for
		// uniformlySampleAnnulus() for derivation.

		Real r =
			std::sqrt(uv[1]);
		const Real theta =

			2 * constantPi<Real>() * uv[0];

		return sphericalToCartesian(
			Vector<Real, 2>(r, theta));
	}
	*/

	template <typename Real, int N>
	PASTEL_ENABLE_IF_C(N == 3, (Vector<Real, N>))
		uniformlySampleBall(
		const Vector<Real, N>& uv)
	{
		// This is the old implementation of 3-dimensional
		// uniform sampling from a ball.
		// Sampling uniformly from a ball is a special case of
		// sampling from an annulus: the unit-ball is a
		// (0, 1)-annulus. See the documentation for
		// uniformlySampleAnnulus() for derivation.

		// TODO: Replace this function with a generalization
		// of the 2d low-distortion map.

		Real r =
			std::pow(uv[0], (Real)1/3);
		const Real phi =

			std::acos(1 - 2 * uv[1]);
		Real theta =

			2 * constantPi<Real>() * uv[2];

		return sphericalToCartesian(
			Vector<Real, 3>(r, phi, theta));
	}

	/*
	// distortDiskToCube

	template <typename Real>
	Vector<Real, 2> distortDiskToCube(
		const Vector<Real, 2>& uv)
	{
		// This is the inverse of the
		// uniformlySampleBall<2, Real>() function.

		Real radius(norm(uv));
		const Real theta(positiveRadians<Real>(std::atan2(uv[1], uv[0])));
		const Real scaledTheta(
			theta / (constantPi<Real>() / 4));

		Real s;
		Real t;

		if (uv.x() > uv.y())
		{
			if (uv.x() > -uv.y())
			{
				// Right triangle
				s = radius;

				t = scaledTheta * radius;
			}
			else
			{
				// Bottom triangle
				s = (scaledTheta - 6) * radius;
				t = -radius;
			}
		}
		else
		{
			if (uv.x() > -uv.y())
			{
				// Top triangle
				s = (2 - scaledTheta) / radius;
				t = radius;
			}
			else
			{
				// Left triangle
				s = -radius;
				t = (4 - scaledTheta) * radius;
			}
		}

		return Vector<Real, 2>(
			(s + 1) / 2, (t + 1) / 2);
	}
	*/

	// uniformlySampleSphere

	template <typename Real, int N>
	PASTEL_ENABLE_IF_C(N == 1, (Vector<Real, ModifyN<N, N + 1>::Result>))
		uniformlySampleSphere(
		const Vector<Real, N>& uv)
	{
		const Real angle = 2 * constantPi<Real>() * uv[0];

		return Vector<Real, 2>(
			std::cos(angle),
			std::sin(angle));
	}

	template <typename Real, int N>
	PASTEL_ENABLE_IF_C(N == 2, (Vector<Real, ModifyN<N, N + 1>::Result>))
		uniformlySampleSphere(
		const Vector<Real, N>& uv)
	{
		const Real phi(2 * constantPi<Real>() * uv[0]);
		const Real z(1 - 2 * uv[1]);
		const Real r(std::sqrt(1 - z * z));

		const Real x(r * std::cos(phi));
		const Real y(r * std::sin(phi));

		return Vector<Real, 3>(x, y, z);
	}

	// uniformlySampleHemisphere

	template <typename Real, int N>
	PASTEL_ENABLE_IF_C(N == 1, (Vector<Real, ModifyN<N, N + 1>::Result>))
		uniformlySampleHemisphere(
		const Vector<Real, N>& uv)
	{
		const Real phi = constantPi<Real>() * uv[0];
		return Vector<Real, 2>(
			std::cos(phi),
			std::sin(phi));
	}

	template <typename Real, int N>
	PASTEL_ENABLE_IF_C(N == 2, (Vector<Real, ModifyN<N, N + 1>::Result>))
		uniformlySampleHemisphere(
		const Vector<Real, N>& uv)
	{
		const Real phi(2 * constantPi<Real>() * uv[0]);
		Real z(uv[1]);

		const Real r(std::sqrt(1 - z * z));
		const Real x(r * std::cos(phi));
		const Real y(r * std::sin(phi));

		return Vector<Real, 3>(x, y, z);
	}

	// cosineSampleHemisphere

	template <typename Real, int N>
	Vector<Real, ModifyN<N, N + 1>::Result> cosineSampleHemisphere(
		const Vector<Real, N>& uv)
	{
		// Malley's method:
		// Projecting a uniformly distributed
		// (n-1)-ball orthogonally up onto a n-hemisphere
		// produces a cosine-weighted distribution.

		Vector<Real, N> ball = uniformlySampleBall(uv);

		return extend(ball, std::sqrt(1 - dot(ball)));
	}

}

#endif
