// Description: Generalized-Gaussian-distributed random numbers

#ifndef PASTEL_RANDOM_GENERALIZEDGAUSSIAN_H
#define PASTEL_RANDOM_GENERALIZEDGAUSSIAN_H

#include "pastel/sys/mytypes.h"

namespace Pastel
{

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
		const PASTEL_NO_DEDUCTION(Real)& shape, 
		const PASTEL_NO_DEDUCTION(Real)& scale);

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
		const PASTEL_NO_DEDUCTION(Real)& shape);

	//! Converts variance to Generalized Gaussian scale parameter.
	/*!
	Preconditions:
	shape > 0
	variance >= 0
	*/
	template <typename Real>
	Real varianceToGeneralizedGaussianScale(
		const PASTEL_NO_DEDUCTION(Real)& shape,
		const PASTEL_NO_DEDUCTION(Real)& variance);

	//! Probability density function of the Generalized Gaussian Distribution.
	/*!
	Preconditions:
	shape > 0
	scale >= 0
	*/
	template <typename Real>
	Real generalizedGaussianPdf(
		const PASTEL_NO_DEDUCTION(Real)& x,
		const PASTEL_NO_DEDUCTION(Real)& shape,
		const PASTEL_NO_DEDUCTION(Real)& scale);

	//! Probability density function of the Generalized Gaussian Distribution.
	/*!
	Preconditions:
	shape > 0
	*/
	template <typename Real>
	Real generalizedGaussianPdf(
		const PASTEL_NO_DEDUCTION(Real)& x,
		const PASTEL_NO_DEDUCTION(Real)& shape);

}

#include "pastel/sys/random_generalizedgaussian.hpp"

#endif
