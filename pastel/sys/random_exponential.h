// Description: Exponential-distributed random numbers

#ifndef PASTEL_RANDOM_EXPONENTIAL_H
#define PASTEL_RANDOM_EXPONENTIAL_H

#include "pastel/sys/mytypes.h"
#include "pastel/sys/vector.h"

namespace Pastel
{

	//! Returns an exponentially-distributed random real.
	/*!
	This function assumes mean = 1.
	*/
	template <typename Real>
	Real randomExponential();

	//! Returns an exponentially-distributed random real.
	/*!
	Precondition:
	mean > 0

	This is a convenience function that calls
	randomExponential<Real>() / mean;
	*/
	template <typename Real>
	Real randomExponential(
		const PASTEL_NO_DEDUCTION(Real)& mean);

	//! Returns a random vector exponentially distributed in each component.
	
	template <int N, typename Real>
	Vector<Real, N> randomExponentialVector();

	//! Returns a random vector exponentially distributed in each component.
	
	template <int N, typename Real>
	Vector<Real, N> randomExponentialVector(integer dimension);

	//! Probability density function of the Exponential distribution.
	/*!
	This function assumes mean = 1.
	*/
	template <typename Real>
	Real exponentialPdf(
		const PASTEL_NO_DEDUCTION(Real)& x);

	//! Probability density function of the Exponential distribution.
	/*!
	Precondition:
	mean > 0
	*/
	template <typename Real>
	Real exponentialPdf(
		const PASTEL_NO_DEDUCTION(Real)& x,
		const PASTEL_NO_DEDUCTION(Real)& mean);

}

#include "pastel/sys/random_exponential.hpp"

#endif
