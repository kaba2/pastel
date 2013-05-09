// Description: Geometrically-distributed random numbers

#ifndef PASTELSYS_RANDOM_GEOMETRIC_H
#define PASTELSYS_RANDOM_GEOMETRIC_H

#include "pastel/sys/mytypes.h"

namespace Pastel
{

	//! Returns a geometrically-distributed random number.
	/*!
	The returned random number is distributed as the number 
	of failures before the first success in a Bernoulli process;
	its support is {0, 1, 2, ...}.
	*/
	template <typename Real>
	integer randomGeometric(
		const PASTEL_NO_DEDUCTION(Real)& success);

	//! Probability mass function of the geometric distribution.
	template <typename Real>
	Real geometricPdf(
		const PASTEL_NO_DEDUCTION(Real)& k,
		const PASTEL_NO_DEDUCTION(Real)& success);

	//! Cumulative distribution function of the geometric distribution.
	template <typename Real>
	Real geometricCdf(
		const PASTEL_NO_DEDUCTION(Real)& k,
		const PASTEL_NO_DEDUCTION(Real)& success);

}

#include "pastel/sys/random_geometric.hpp"

#endif
