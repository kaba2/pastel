// Description: Uniform sampling

#ifndef PASTEL_UNIFORM_SAMPLING_H
#define PASTEL_UNIFORM_SAMPLING_H

#include "pastel/sys/vector.h"

namespace Pastel
{

	//! Returns a random vector uniformly distributed in [-1, 1]^N.

	template <typename Real, int N>
	Vector<Real, N> randomVectorCube();

	//! Returns a random vector uniformly distributed in [-1, 1]^N.
	/*!
	Preconditions:
	dimension >= 0
	*/

	template <typename Real, int N>
	Vector<Real, N> randomVectorCube(integer dimension);

	//! Returns a random vector uniformly distributed on the unit sphere.
	/*!
	Returns:
	A vector x such that dot(x) = 1.
	*/

	template <typename Real, int N>
	Vector<Real, N> randomVectorSphere();

	//! Returns a random vector uniformly distributed on the unit sphere.
	/*!
	Preconditions:
	dimension >= 0

	Returns:
	A vector x such that dot(x) = 1.
	*/

	template <typename Real, int N>
	Vector<Real, N> randomVectorSphere(integer dimension);

	//! Returns a random vector uniformly distributed in the unit ball.
	/*!
	Returns:
	A vector x such that dot(x) <= 1.
	*/

	template <typename Real, int N>
	Vector<Real, N> randomVectorBall();

	//! Returns a random vector uniformly distributed in the unit ball.
	/*!
	Preconditions:
	dimension >= 0

	Returns:
	A vector x such that dot(x) <= 1.
	*/

	template <typename Real, int N>
	Vector<Real, N> randomVectorBall(integer dimension);

	//! Returns a random vector uniformly distributed on the annulus.
	/*!
	Returns:
	A vector x such that minRadius <= norm(x) <= maxRadius.
	*/

	template <typename Real, int N>
	Vector<Real, N> randomVectorAnnulus(
		const PASTEL_NO_DEDUCTION(Real)& minRadius,
		const PASTEL_NO_DEDUCTION(Real)& maxRadius);

	//! Returns a random vector uniformly distributed on the annulus.
	/*!
	Preconditions:
	dimension >= 0

	Returns:
	A vector x such that minRadius <= norm(x) <= maxRadius.
	*/

	template <typename Real, int N>
	Vector<Real, N> randomVectorAnnulus(
		const PASTEL_NO_DEDUCTION(Real)& minRadius,
		const PASTEL_NO_DEDUCTION(Real)& maxRadius,
		integer dimension);

}

#include "pastel/math/uniform_sampling.hpp"

#endif
