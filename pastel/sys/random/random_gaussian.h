// Description: Gaussian-distributed random numbers

#ifndef PASTELSYS_RANDOM_GAUSSIAN_H
#define PASTELSYS_RANDOM_GAUSSIAN_H

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
		const NoDeduction<Real>& deviation);

	//! Returns a random vector standard-normally distributed in a hyperball.
	
	template <typename Real, integer N>
	Vector<Real, N> randomGaussianVector();

	//! Returns a random vector standard-normally distributed in a hyperball.
	
	template <typename Real, integer N>
	Vector<Real, N> randomGaussianVector(integer dimension);

	//! Probability density function of the Gaussian distribution.
	/*!
	This function assumes deviation = 1.
	*/
	template <typename Real>
	Real gaussianPdf(
		const NoDeduction<Real>& x);

	//! Probability density function of the Gaussian distribution.
	template <typename Real>
	Real gaussianPdf(
		const NoDeduction<Real>& x,
		const NoDeduction<Real>& deviation);

	//! Cumulative distribution function of the Gaussian distribution.
	template <typename Real>
	Real approximateGaussianCdf(
		const NoDeduction<Real>& x,
		const NoDeduction<Real>& deviation);

	//! Cumulative distribution function of the Gaussian distribution.
	/*!
	This function assumes deviation = 1.
	*/
	template <typename Real>
	Real approximateGaussianCdf(
		const NoDeduction<Real>& x);

	//! Cumulative distribution function of the Gaussian distribution.
	/*!
	This function assumes deviation = 1.
	*/
	template <typename Real>
	Real gaussianCdf(
		const NoDeduction<Real>& x);

	//! Cumulative distribution function of the Gaussian distribution.
	template <typename Real>
	Real gaussianCdf(
		const NoDeduction<Real>& x,
		const NoDeduction<Real>& deviation);

}

#include "pastel/sys/random/random_gaussian.hpp"

#endif
