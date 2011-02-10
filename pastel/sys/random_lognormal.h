// Description: Log-normal-distributed random numbers
// Documentation: random_continuous.txt

#ifndef PASTEL_RANDOM_LOGNORMAL_H
#define PASTEL_RANDOM_LOGNORMAL_H

#include "pastel/sys/mytypes.h"

namespace Pastel
{

	//! Returns a Log-normal-distributed random real.
	/*!
	This function assumes logMean = 0 and logDeviation = 1.
	*/
	template <typename Real>
	Real randomLogNormal();

	//! Returns a Log-normal-distributed random real.
	/*!
	Preconditions:
	logDeviation > 0
	*/
	template <typename Real>
	Real randomLogNormal(
		const PASTEL_NO_DEDUCTION(Real)& logMean,
		const PASTEL_NO_DEDUCTION(Real)& logDeviation);

	// Probability density function of the Log-normal distribution.
	/*!
	This function assumes logMean = 0 and logDeviation = 1.
	*/
	template <typename Real>
	Real logNormalPdf(
		const PASTEL_NO_DEDUCTION(Real)& x);

	// Probability density function of the Log-normal distribution.
	/*!
	Preconditions:
	logDeviation > 0
	*/
	template <typename Real>
	Real logNormalPdf(
		const PASTEL_NO_DEDUCTION(Real)& x,
		const PASTEL_NO_DEDUCTION(Real)& logMean,
		const PASTEL_NO_DEDUCTION(Real)& logDeviation);

}

#include "pastel/sys/random_lognormal.hpp"

#endif
