// Description: Beta-distributed random numbers
// Documentation: random_continuous.txt

#ifndef PASTEL_RANDOM_BETA_H
#define PASTEL_RANDOM_BETA_H

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
		const PASTEL_NO_DEDUCTION(Real)& a,
		const PASTEL_NO_DEDUCTION(Real)& b);

	// Probability distribution of the Beta distribution.
	template <typename Real>
	Real betaPdf(
		const PASTEL_NO_DEDUCTION(Real)& x,
		const PASTEL_NO_DEDUCTION(Real)& a,
		const PASTEL_NO_DEDUCTION(Real)& b);

}

#include "pastel/sys/random_beta.hpp"

#endif
