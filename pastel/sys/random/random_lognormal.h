// Description: Log-normal-distributed random numbers

#ifndef PASTELSYS_RANDOM_LOGNORMAL_H
#define PASTELSYS_RANDOM_LOGNORMAL_H

#include "pastel/sys/mytypes.h"

namespace Pastel
{

	//! Returns a Log-normal-distributed random dreal.
	/*!
	This function assumes logMean = 0 and logDeviation = 1.
	*/
	template <typename Real>
	Real randomLogNormal();

	//! Returns a Log-normal-distributed random dreal.
	/*!
	Preconditions:
	logDeviation > 0
	*/
	template <typename Real>
	Real randomLogNormal(
		const NoDeduction<Real>& logMean,
		const NoDeduction<Real>& logDeviation);

	// Probability density function of the Log-normal distribution.
	/*!
	This function assumes logMean = 0 and logDeviation = 1.
	*/
	template <typename Real>
	Real logNormalPdf(
		const NoDeduction<Real>& x);

	// Probability density function of the Log-normal distribution.
	/*!
	Preconditions:
	logDeviation > 0
	*/
	template <typename Real>
	Real logNormalPdf(
		const NoDeduction<Real>& x,
		const NoDeduction<Real>& logMean,
		const NoDeduction<Real>& logDeviation);

}

#include "pastel/sys/random/random_lognormal.hpp"

#endif
