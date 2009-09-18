#ifndef PASTEL_RANDOM_BETA_H
#define PASTEL_RANDOM_BETA_H

#include "pastel/sys/mytypes.h"

namespace Pastel
{

	//! Returns a beta-distributed random real.
	/*!
	Preconditions:
	alpha > 0
	beta > 0
	*/
	template <typename Real>
	Real randomBeta(
		const PASTEL_NO_DEDUCTION(Real)& alpha,
		const PASTEL_NO_DEDUCTION(Real)& beta);

}

#include "pastel/sys/random_beta.hpp"

#endif
