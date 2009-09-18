#ifndef PASTEL_RANDOM_BETA_HPP
#define PASTEL_RANDOM_BETA_HPP

#include "pastel/sys/random_beta.h"
#include "pastel/sys/random_gamma.h"

namespace Pastel
{

	template <typename Real>
	Real randomBeta(
		const PASTEL_NO_DEDUCTION(Real)& alpha,
		const PASTEL_NO_DEDUCTION(Real)& beta)
	{
		PENSURE_OP(alpha, >, 0);
		PENSURE_OP(beta, >, 0);

		const Real u = randomGamma<Real>(alpha);
		const Real v = randomGamma<Real>(beta);

		return u / (u + v);
	}

}

#endif
