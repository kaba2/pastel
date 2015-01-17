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

	Preconditions:
	success > 0
	success < 1
	*/
	template <typename Real>
	integer randomGeometric(
		const NoDeduction<Real>& success);

	//! Probability mass function of the geometric distribution.
	/*!
	Preconditions:
	k >= 0
	success > 0
	success < 1
	*/
	template <typename Real>
	Real geometricPdf(
		const NoDeduction<Real>& k,
		const NoDeduction<Real>& success);

	//! Cumulative distribution function of the geometric distribution.
	/*!
	Preconditions:
	k >= 0
	success > 0
	success < 1
	*/
	template <typename Real>
	Real geometricCdf(
		const NoDeduction<Real>& k,
		const NoDeduction<Real>& success);

}

#include "pastel/sys/random/random_geometric.hpp"

#endif
