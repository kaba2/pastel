// Description: Gaussian-distributed random numbers

#ifndef PASTELSYS_RANDOM_GAUSSIAN_H
#define PASTELSYS_RANDOM_GAUSSIAN_H

#include "pastel/sys/mytypes.h"
#include "pastel/sys/vector.h"

namespace Pastel
{

	//! Returns a normal-distributed random dreal.
	template <typename Real>
	Real randomGaussian();

	//! Returns a normal-distributed random dreal.
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

#include "pastel/sys/random/normal_distribution.h"
#include "pastel/sys/random/normal_entropy.h"
#include "pastel/sys/random/normal_moments.h"
#include "pastel/sys/random/normal_mutual_information.h"
#include "pastel/sys/random/normal_pdf.h"
#include "pastel/sys/random/normal_variate.h"

#endif
