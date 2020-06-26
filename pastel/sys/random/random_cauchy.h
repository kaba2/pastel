// Description: Cauchy-distributed random numbers

#ifndef PASTELSYS_RANDOM_CAUCHY_H
#define PASTELSYS_RANDOM_CAUCHY_H

#include "pastel/sys/mytypes.h"

namespace Pastel
{

	//! Returns a Cauchy-distributed random dreal.
	/*!
	This function assumes scale = 1.
	*/
	template <typename Real>
	Real randomCauchy();

	//! Returns a Cauchy-distributed random dreal.
	/*!
	Preconditions:
	scale > 0

	This a convenience function that calls
	randomCauchy<Real>(x) * scale;
	*/
	template <typename Real>
	Real randomCauchy(
		const NoDeduction<Real>& scale);

	//! Probability density function of the Cauchy distribution.
	/*!
	This function assumes scale = 1.
	*/
	template <typename Real>
	Real cauchyPdf(
		const NoDeduction<Real>& x);

	//! Probability density function of the Cauchy distribution.
	/*!
	Preconditions:
	scale > 0

	This a convenience function that calls
	cauchyPdf<Real>(x) / scale;
	*/
	template <typename Real>
	Real cauchyPdf(
		const NoDeduction<Real>& x,
		const NoDeduction<Real>& scale);

}

#include "pastel/sys/random/random_cauchy.hpp"

#endif
