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

}

#include "pastel/sys/random_laplace.hpp"

#endif
