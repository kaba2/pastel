// Description: Beta-distributed random numbers

#ifndef PASTELSYS_RANDOM_BETA_H
#define PASTELSYS_RANDOM_BETA_H

#include "pastel/sys/mytypes.h"

namespace Pastel
{

	//! Returns a beta-distributed random real.
	/*!
	Preconditions:
	a > 0
	b > 0
	*/
	template <typename Real>
	Real randomBeta(
		const NoDeduction<Real>& a,
		const NoDeduction<Real>& b);

	// Probability distribution of the Beta distribution.
	template <typename Real>
	Real betaPdf(
		const NoDeduction<Real>& x,
		const NoDeduction<Real>& a,
		const NoDeduction<Real>& b);

}

#include "pastel/sys/random/random_beta.hpp"

#endif
