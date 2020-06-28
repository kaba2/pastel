// Description: Exponential-distributed random numbers

#ifndef PASTELSYS_RANDOM_EXPONENTIAL_H
#define PASTELSYS_RANDOM_EXPONENTIAL_H

#include "pastel/sys/mytypes.h"
#include "pastel/sys/vector.h"

namespace Pastel
{

	//! Returns an exponentially-distributed random dreal.
	/*!
	This function assumes mean = 1.
	*/
	template <typename Real>
	Real randomExponential();

	//! Returns an exponentially-distributed random dreal.
	/*!
	Precondition:
	mean > 0

	This is a convenience function that calls
	randomExponential<Real>() / mean;
	*/
	template <typename Real>
	Real randomExponential(
		const NoDeduction<Real>& mean);

	//! Returns a random vector exponentially distributed in each component.
	
	template <typename Real, int N>
	Vector<Real, N> randomExponentialVector();

	//! Returns a random vector exponentially distributed in each component.
	
	template <typename Real, int N>
	Vector<Real, N> randomExponentialVector(integer dimension);

	//! Probability density function of the Exponential distribution.
	/*!
	This function assumes mean = 1.
	*/
	template <typename Real>
	Real exponentialPdf(
		const NoDeduction<Real>& x);

	//! Probability density function of the Exponential distribution.
	/*!
	Precondition:
	mean > 0
	*/
	template <typename Real>
	Real exponentialPdf(
		const NoDeduction<Real>& x,
		const NoDeduction<Real>& mean);

}

#include "pastel/sys/random/random_exponential.hpp"

#endif
