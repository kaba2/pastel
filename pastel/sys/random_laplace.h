// Description: Laplace-distributed random numbers

#ifndef PASTEL_RANDOM_LAPLACE_H
#define PASTEL_RANDOM_LAPLACE_H

#include "pastel/sys/mytypes.h"

namespace Pastel
{

	//! Returns a Laplace-distributed random real.
	/*!
	This function assumes that the scale parameter is 1.
	*/
	template <typename Real>
	Real randomLaplace();

	//! Returns a Laplace-distributed random real.
	/*!
	Preconditions:
	scale > 0

	This is a convenience function that calls
	randomLaplace<Real>() * scale;
	*/
	template <typename Real>
	Real randomLaplace(
		const PASTEL_NO_DEDUCTION(Real)& scale);

	//! Probability distribution of the Laplace distribution.
	/*!
	This function assumes scale = 1.
	*/
	template <typename Real>
	Real laplacePdf(
		const PASTEL_NO_DEDUCTION(Real)& x);

	//! Probability distribution of the Laplace distribution.
	/*!
	This is a convenience function that calls:
	laplacePdf<Real>(x) / scale;
	*/
	template <typename Real>
	Real laplacePdf(
		const PASTEL_NO_DEDUCTION(Real)& x,
		const PASTEL_NO_DEDUCTION(Real)& scale);

}

#include "pastel/sys/random_laplace.hpp"

#endif
