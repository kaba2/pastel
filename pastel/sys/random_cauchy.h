// Description: Cauchy-distributed random numbers

#ifndef PASTEL_RANDOM_CAUCHY_H
#define PASTEL_RANDOM_CAUCHY_H

#include "pastel/sys/mytypes.h"

namespace Pastel
{

	//! Returns a Cauchy-distributed random real.
	/*!
	This function assumes and scale = 1.
	*/
	template <typename Real>
	Real randomCauchy();

	//! Returns a Cauchy-distributed random real.
	/*!
	Preconditions:
	scale > 0

	This a convenience function that calls
	randomCauchy<Real>(x) * scale;
	*/
	template <typename Real>
	Real randomCauchy(
		const PASTEL_NO_DEDUCTION(Real)& scale);

	//! Probability density function of the Cauchy distribution.
	/*!
	This function assumes scale = 1.
	*/
	template <typename Real>
	Real cauchyPdf(
		const PASTEL_NO_DEDUCTION(Real)& x);

	//! Probability density function of the Cauchy distribution.
	/*!
	Preconditions:
	scale > 0

	This a convenience function that calls
	cauchyPdf<Real>(x) / scale;
	*/
	template <typename Real>
	Real cauchyPdf(
		const PASTEL_NO_DEDUCTION(Real)& x,
		const PASTEL_NO_DEDUCTION(Real)& scale);

}

#include "pastel/sys/random_cauchy.hpp"

#endif
