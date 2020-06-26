// Description: Laplace-distributed random numbers

#ifndef PASTELSYS_RANDOM_LAPLACE_H
#define PASTELSYS_RANDOM_LAPLACE_H

#include "pastel/sys/mytypes.h"

namespace Pastel
{

	//! Returns a Laplace-distributed random dreal.
	/*!
	This function assumes that the scale parameter is 1.
	*/
	template <typename Real>
	Real randomLaplace();

	//! Returns a Laplace-distributed random dreal.
	/*!
	Preconditions:
	scale > 0

	This is a convenience function that calls
	randomLaplace<Real>() * scale;
	*/
	template <typename Real>
	Real randomLaplace(
		const NoDeduction<Real>& scale);

	//! Probability distribution of the Laplace distribution.
	/*!
	This function assumes scale = 1.
	*/
	template <typename Real>
	Real laplacePdf(
		const NoDeduction<Real>& x);

	//! Probability distribution of the Laplace distribution.
	/*!
	This is a convenience function that calls:
	laplacePdf<Real>(x) / scale;
	*/
	template <typename Real>
	Real laplacePdf(
		const NoDeduction<Real>& x,
		const NoDeduction<Real>& scale);

}

#include "pastel/sys/random/random_laplace.hpp"

#endif
