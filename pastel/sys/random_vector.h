#ifndef PASTEL_RANDOM_VECTOR_H
#define PASTEL_RANDOM_VECTOR_H

#include "pastel/sys/vector.h"

namespace Pastel
{

	//! Returns a random vector uniformly distributed in [-1, 1]^N.

	template <int N, typename Real>
	TemporaryVector<N, Real> randomVectorCube();

	//! Returns a random vector uniformly distributed in [-1, 1]^N.
	/*!
	Preconditions:
	dimension >= 0
	*/

	template <int N, typename Real>
	TemporaryVector<N, Real> randomVectorCube(integer dimension);

	//! Returns a random vector uniformly distributed on the unit sphere.
	/*!
	Returns:
	A vector x such that dot(x) = 1.
	*/

	template <int N, typename Real>
	TemporaryVector<N, Real> randomVectorSphere();

	//! Returns a random vector uniformly distributed on the unit sphere.
	/*!
	Preconditions:
	dimension >= 0

	Returns:
	A vector x such that dot(x) = 1.
	*/

	template <int N, typename Real>
	TemporaryVector<N, Real> randomVectorSphere(integer dimension);

	//! Returns a random vector uniformly distributed in the unit ball.
	/*!
	Returns:
	A vector x such that dot(x) <= 1.
	*/

	template <int N, typename Real>
	TemporaryVector<N, Real> randomVectorBall();

	//! Returns a random vector uniformly distributed in the unit ball.
	/*!
	Preconditions:
	dimension >= 0

	Returns:
	A vector x such that dot(x) <= 1.
	*/

	template <int N, typename Real>
	TemporaryVector<N, Real> randomVectorBall(integer dimension);

	//! Returns a random vector uniformly distributed on the annulus.
	/*!
	Returns:
	A vector x such that minRadius <= norm(x) <= maxRadius.
	*/

	template <int N, typename Real>
	TemporaryVector<N, Real> randomVectorAnnulus(
		const PASTEL_NO_DEDUCTION(Real)& minRadius,
		const PASTEL_NO_DEDUCTION(Real)& maxRadius);

	//! Returns a random vector uniformly distributed on the annulus.
	/*!
	Preconditions:
	dimension >= 0

	Returns:
	A vector x such that minRadius <= norm(x) <= maxRadius.
	*/

	template <int N, typename Real>
	TemporaryVector<N, Real> randomVectorAnnulus(
		const PASTEL_NO_DEDUCTION(Real)& minRadius,
		const PASTEL_NO_DEDUCTION(Real)& maxRadius,
		integer dimension);

	//! Returns a random vector uniformly distributed in [0, 1]^N.

	template <int N, typename Real>
	TemporaryVector<N, Real> randomVector();

	//! Returns a random vector uniformly distributed in [0, 1]^N.
	/*!
	Preconditions:
	dimension >= 0
	*/

	template <int N, typename Real>
	TemporaryVector<N, Real> randomVector(integer dimension);

	//! Returns a random vector standard-normally distributed in a hyperball.
	
	template <int N, typename Real>
	TemporaryVector<N, Real> randomGaussianVector();

	//! Returns a random vector standard-normally distributed in a hyperball.
	
	template <int N, typename Real>
	TemporaryVector<N, Real> randomGaussianVector(integer dimension);

	//! Returns a random vector exponentially distributed in each component.
	
	template <int N, typename Real>
	TemporaryVector<N, Real> randomExponentialVector();

	//! Returns a random vector exponentially distributed in each component.
	
	template <int N, typename Real>
	TemporaryVector<N, Real> randomExponentialVector(integer dimension);

	//! Returns a random vector gamma distributed in each component.
	
	template <int N, typename Real>
	TemporaryVector<N, Real> randomGammaVector(
		const PASTEL_NO_DEDUCTION(Real)& scale);

	//! Returns a random vector gamma distributed in each component.
	
	template <int N, typename Real>
	TemporaryVector<N, Real> randomGammaVector(
		integer dimension,
		const PASTEL_NO_DEDUCTION(Real)& scale);

	template <int N, typename Real>
	TemporaryVector<N, Real> randomGeneralizedGaussianVector(
		const PASTEL_NO_DEDUCTION(Real)& shape, 
		const PASTEL_NO_DEDUCTION(Real)& scale);

	template <int N, typename Real>
	TemporaryVector<N, Real> randomGeneralizedGaussianVector(
		integer dimension,
		const PASTEL_NO_DEDUCTION(Real)& shape, 
		const PASTEL_NO_DEDUCTION(Real)& scale);

}

#include "pastel/sys/random_vector.hpp"

#endif
