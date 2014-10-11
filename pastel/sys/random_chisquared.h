// Description: Chi-squared-distributed random numbers

#ifndef PASTELSYS_RANDOM_CHISQUARED_H
#define PASTELSYS_RANDOM_CHISQUARED_H

#include "pastel/sys/mytypes.h"

namespace Pastel
{

	//! Returns a Chi-squared distributed random variable.
	/*!
	Preconditions:
	k > 0
	*/
	template <typename Real>
	Real randomChiSquared(integer k);

	//! Probability density function of the Chi-squared distribution.
	/*!
	Preconditions:
	k > 0
	*/
	template <typename Real>
	Real chiSquaredPdf(
		const NoDeduction<Real>& x,
		integer k);

}

#include "pastel/sys/random_chisquared.hpp"

#endif
