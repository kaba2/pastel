#ifndef PASTELSYS_RANDOM_GEOMETRIC_HPP
#define PASTELSYS_RANDOM_GEOMETRIC_HPP

#include "pastel/sys/random_geometric.h"
#include "pastel/sys/random_uniform.h"

#include <cmath>

namespace Pastel
{

	template <typename Real>
	integer randomGeometric(
		const PASTEL_NO_DEDUCTION(Real)& success)
	{
		PENSURE_OP(success, >, 0);
		PENSURE_OP(success, <, 1);

		// Generates an exponentially distributed
		// random number, and rounds it down.
		return std::log(randomOpen0<Real>()) / 
			std::log(1 - success);
	}

	template <typename Real>
	Real geometricPdf(
		const PASTEL_NO_DEDUCTION(Real)& k,
		const PASTEL_NO_DEDUCTION(Real)& success)
	{
		PENSURE_OP(success, >, 0);
		PENSURE_OP(success, <, 1);

		// The probability of having a k consecutive
		// failures before the first success in a
		// Bernoulli process.
		return std::pow(1 - success, k) * success;
	}

	template <typename Real>
	Real geometricCdf(
		const PASTEL_NO_DEDUCTION(Real)& k,
		const PASTEL_NO_DEDUCTION(Real)& success)
	{
		PENSURE_OP(success, >, 0);
		PENSURE_OP(success, <, 1);

		// The probability of having at most k
		// consecutive failures before the first
		// success in a Bernoulli process.
		return 1 - std::pow(1 - success, k + 1);
	}

}

#endif
