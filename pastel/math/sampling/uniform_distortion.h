// Description: Uniform distortion

#ifndef PASTELMATH_UNIFORM_DISTORTION_H
#define PASTELMATH_UNIFORM_DISTORTION_H

#include "pastel/sys/vector.h"

namespace Pastel
{

	//! Uniformly samples a point from an annulus
	/*!
	Preconditions:
	'uv' is in [0, 1]^N.
	minRadius >= 0
	maxRadius >= 0
	minRadius < maxRadius

	The annulus is considered as the set:
	{x | minRadius <= norm(x) <= minRadius}

	If 'uv' are random variables uniformly distributed
	on the square, then the returned points are random
	variables uniformly distributed on the annulus.
	*/

	template <typename Real, int N, RequiresC<(N == 1)> = 0>
	Vector<Real, N> uniformlySampleAnnulus(
		const Vector<Real, N>& uv,
		const NoDeduction<Real>& minRadius,
		const NoDeduction<Real>& maxRadius);

	template <typename Real, int N, RequiresC<(N == 2)> = 0>
	Vector<Real, N> uniformlySampleAnnulus(
		const Vector<Real, N>& uv,
		const NoDeduction<Real>& minRadius,
		const NoDeduction<Real>& maxRadius);

	template <typename Real, int N, RequiresC<(N == 3)> = 0>
	Vector<Real, N> uniformlySampleAnnulus(
		const Vector<Real, N>& uv,
		const NoDeduction<Real>& minRadius,
		const NoDeduction<Real>& maxRadius);

	//! Uniformly samples a point from a simplex.
	/*!
	Preconditions:
	'uv' is in [0, 1]^N.

	The simplex is considered as the set
	{x | sum(x) <= 1, allGreaterEqual(x, 0)}

	If 'uv' are random variables uniformly distributed
	on the cube, then the returned points are random
	variables uniformly distributed on the simplex.
	*/

	template <typename Real, int N, RequiresC<(N == 1)> = 0>
	Vector<Real, N> uniformlySampleSimplex(
		const Vector<Real, N>& uv);

	template <typename Real, int N, RequiresC<(N == 2)> = 0>
	Vector<Real, N> uniformlySampleSimplex(
		const Vector<Real, N>& uv);

	template <typename Real, int N, RequiresC<(N >= 3 || N == Dynamic)> = 0>
	Vector<Real, N> uniformlySampleSimplex(
		const Vector<Real, N>& uv);

	//! Uniformly samples a point from a ball.
	/*!
	Preconditions:
	'uv' is in [0, 1]^N.

	Returns:
	A point from the set {x in R^N | norm(x) <= 1}.

	If 'uv' is a uniformly distributed random variable on the cube,
	so is the returned value on the ball.
	*/

	template <typename Real, int N, RequiresC<(N == 1)> = 0>
	Vector<Real, N> uniformlySampleBall(
		const Vector<Real, N>& uv);

	template <typename Real, int N, RequiresC<(N == 2)> = 0>
	Vector<Real, N> uniformlySampleBall(
		const Vector<Real, N>& uv);

	template <typename Real, int N, RequiresC<(N == 3)> = 0>
	Vector<Real, N> uniformlySampleBall(
		const Vector<Real, N>& uv);

	// TODO: provide functions for 4d and up?

	//! Uniformly samples a point from a sphere.
	/*!
	Preconditions:
	'uv' is in [0, 1]^N

	Returns:
	A point from the set {x in R^(N + 1) | norm(x) = 1}.

	If 'uv' is a uniformly distributed random variable on the cube,
	so is the returned value on the sphere.
	*/

	template <typename Real, int N, RequiresC<(N == 1)> = 0>
	Vector<Real, ModifyN<N, N + 1>::Result> uniformlySampleSphere(
		const Vector<Real, N>& uv);

	template <typename Real, int N, RequiresC<(N == 2)> = 0>
	Vector<Real, ModifyN<N, N + 1>::Result> uniformlySampleSphere(
		const Vector<Real, N>& uv);

	// TODO: provide functions for 4d and up?

	//! Uniformly samples a point from a hemisphere.
	/*!
	Preconditions:
	'uv' is in [0, 1]^N.

	Returns:
	A point from the set {x in R^(N + 1) | norm(x) <= 1, x[N] >= 0}.

	If 'uv' is a uniformly distributed random variable on the cube,
	so is the returned value on the hemisphere.
	*/

	template <typename Real, int N, RequiresC<(N == 1)> = 0>
	Vector<Real, ModifyN<N, N + 1>::Result>
		uniformlySampleHemisphere(
		const Vector<Real, N>& uv);

	template <typename Real, int N, RequiresC<(N == 2)> = 0>
	Vector<Real, ModifyN<N, N + 1>::Result>
		uniformlySampleHemisphere(
		const Vector<Real, N>& uv);

	// TODO: provide functions for 4d and up?

	//! Samples a cosine-weighted point from a hemisphere.
	/*!
	Preconditions:
	'uv' is in [0, 1]^N.

	The hemisphere is considered as the set
	{x in R^(N+1) | norm(x) = 1, x[N] >= 0}

	If 'uv' are random variables uniformly distributed
	on the square, then the returned points are random
	variables cosine-distributed on the hemisphere.
	*/

	template <typename Real, int N>
	Vector<Real, ModifyN<N, N + 1>::Result> cosineSampleHemisphere(
		const Vector<Real, N>& uv);

}

#include "pastel/math/sampling/uniform_distortion.hpp"

#endif
