// Description: Gaussian-distributed random numbers

#ifndef PASTEL_RANDOM_GAUSSIAN_H
#define PASTEL_RANDOM_GAUSSIAN_H

#include "pastel/sys/mytypes.h"
#include "pastel/sys/vector.h"

namespace Pastel
{

	//! Returns a normal-distributed random real.
	template <typename Real>
	Real randomGaussian();

	//! Returns a normal-distributed random real.
	/*!
	Precondition:
	deviation >= 0

	This is a convenience function that calls
	randomGaussian<Real>() * deviation;
	*/
	template <typename Real>
	Real randomGaussian(
		const PASTEL_NO_DEDUCTION(Real)& deviation);

	//! Returns a random vector standard-normally distributed in a hyperball.
	
	template <typename Real, int N>
	Vector<Real, N> randomGaussianVector();

	//! Returns a random vector standard-normally distributed in a hyperball.
	
	template <typename Real, int N>
	Vector<Real, N> randomGaussianVector(integer dimension);

	//! Probability density function of the Gaussian distribution.
	/*!
	This function assumes deviation = 1.
	*/
	template <typename Real>
	Real gaussianPdf(
		const PASTEL_NO_DEDUCTION(Real)& x);

	//! Probability density function of the Gaussian distribution.
	template <typename Real>
	Real gaussianPdf(
		const PASTEL_NO_DEDUCTION(Real)& x,
		const PASTEL_NO_DEDUCTION(Real)& deviation);

	//! Cumulative distribution function of the Gaussian distribution.
	template <typename Real>
	Real approximateGaussianCdf(
		const PASTEL_NO_DEDUCTION(Real)& x,
		const PASTEL_NO_DEDUCTION(Real)& deviation);

	//! Cumulative distribution function of the Gaussian distribution.
	/*!
	This function assumes deviation = 1.
	*/
	template <typename Real>
	Real approximateGaussianCdf(
		const PASTEL_NO_DEDUCTION(Real)& x);

	//! Cumulative distribution function of the Gaussian distribution.
	/*!
	This function assumes deviation = 1.
	*/
	template <typename Real>
	Real gaussianCdf(
		const PASTEL_NO_DEDUCTION(Real)& x);

	//! Cumulative distribution function of the Gaussian distribution.
	template <typename Real>
	Real gaussianCdf(
		const PASTEL_NO_DEDUCTION(Real)& x,
		const PASTEL_NO_DEDUCTION(Real)& deviation);

}

#include "pastel/sys/random_gaussian.hpp"

#endif
