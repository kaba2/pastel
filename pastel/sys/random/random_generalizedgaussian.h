// Description: Generalized-Gaussian-distributed random numbers

#ifndef PASTELSYS_RANDOM_GENERALIZEDGAUSSIAN_H
#define PASTELSYS_RANDOM_GENERALIZEDGAUSSIAN_H

#include "pastel/sys/mytypes.h"
#include "pastel/sys/vector.h"

namespace Pastel
{

	//! Returns a generalized-gaussian-distributed random real.
	/*!
	Preconditions:
	shape > 0

	This function assumes that the scale-parameter is 1.
	The Generalized Gaussian distribution corresponds
	to some well-known distributions on specific values
	of 'shape'. These are:
	* shape = 1: Laplace distribution.
	* shape = 2: Gaussian distribution (deviation 1 / sqrt(2)).
	* shape -> oo: Uniform distribution on [-1, 1] in limit.
	*/
	template <typename Real>
	Real randomGeneralizedGaussian(
		const NoDeduction<Real>& shape);

	//! Returns a generalized-gaussian-distributed random real.
	/*!
	Preconditions:
	shape > 0
	scale >= 0

	This is a convenience function that calls:
	randomGeneralizedGaussian<Real>(shape) * scale.
	
	See the documentation for that function.
	*/
	template <typename Real>
	Real randomGeneralizedGaussian(
		const NoDeduction<Real>& shape, 
		const NoDeduction<Real>& scale);

	//! Converts variance to Generalized Gaussian scale parameter.
	/*!
	Preconditions:
	shape > 0
	variance >= 0
	*/
	template <typename Real>
	Real varianceToGeneralizedGaussianScale(
		const NoDeduction<Real>& shape,
		const NoDeduction<Real>& variance);

	//! Returns a vector of Generalized Gaussian random numbers.
	/*!
	Preconditions:
	shape > 0
	scale >= 0
	*/
	template <typename Real, integer N>
	Vector<Real, N> randomGeneralizedGaussianVector(
		const NoDeduction<Real>& shape, 
		const NoDeduction<Real>& scale);

	//! Returns a vector of Generalized Gaussian random numbers.
	/*!
	Preconditions:
	dimension > 0
	shape > 0
	scale >= 0
	*/
	template <typename Real, integer N>
	Vector<Real, N> randomGeneralizedGaussianVector(
		integer dimension,
		const NoDeduction<Real>& shape, 
		const NoDeduction<Real>& scale);

	//! Probability density function of the Generalized Gaussian Distribution.
	/*!
	Preconditions:
	shape > 0
	scale >= 0
	*/
	template <typename Real>
	Real generalizedGaussianPdf(
		const NoDeduction<Real>& x,
		const NoDeduction<Real>& shape,
		const NoDeduction<Real>& scale);

	//! Probability density function of the Generalized Gaussian Distribution.
	/*!
	Preconditions:
	shape > 0
	*/
	template <typename Real>
	Real generalizedGaussianPdf(
		const NoDeduction<Real>& x,
		const NoDeduction<Real>& shape);

}

#include "pastel/sys/random/random_generalizedgaussian.hpp"

#endif
