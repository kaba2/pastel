// Description: Log-normal-distributed random numbers

#ifndef PASTEL_RANDOM_LOGNORMAL_H
#define PASTEL_RANDOM_LOGNORMAL_H

#include "pastel/sys/mytypes.h"

namespace Pastel
{

	//! Returns a Log-normal-distributed random real.
	/*!
	This function assumes normalMean = 0 and normalDeviation = 1.
	*/
	template <typename Real>
	Real randomLogNormal();

	//! Returns a Log-normal-distributed random real.
	/*!
	Preconditions:
	normalDeviation > 0
	*/
	template <typename Real>
	Real randomLogNormal(
		const PASTEL_NO_DEDUCTION(Real)& normalMean,
		const PASTEL_NO_DEDUCTION(Real)& normalDeviation);

	// Probability density function of the Log-normal distribution.
	/*!
	This function assumes normalMean = 0 and normalDeviation = 1.
	*/
	template <typename Real>
	Real logNormalPdf(
		const PASTEL_NO_DEDUCTION(Real)& x);

	// Probability density function of the Log-normal distribution.
	/*!
	Preconditions:
	normalDeviation > 0
	*/
	template <typename Real>
	Real logNormalPdf(
		const PASTEL_NO_DEDUCTION(Real)& x,
		const PASTEL_NO_DEDUCTION(Real)& normalMean,
		const PASTEL_NO_DEDUCTION(Real)& normalDeviation);

}

#include "pastel/sys/random_lognormal.hpp"

#endif
