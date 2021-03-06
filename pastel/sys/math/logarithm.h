// Description: Logarithms
// Documentation: math_functions.txt

#ifndef PASTELSYS_LOGARITHM_H
#define PASTELSYS_LOGARITHM_H

#include "pastel/sys/integer/finite_integer_concept.h"
#include "pastel/sys/real/real_concept.h"

namespace Pastel
{

	//! Computes floor(log_2(x)).
	/*!
	Preconditions:
	x >= 1
	*/
	template <typename Real>
	integer floorLog2(
		NoDeduction<Real> x);

	//! Computes the [base-2 logarithm] of x.
	/*!
	Preconditions:
	x >= 0
	*/
	template <typename Real>
	Real log2(
		const NoDeduction<Real>& x);

	//! Returns floor(log_2(that)).
	/*!
	Time complexity:
	O(log(bits(that))),

	Preconditions:
	that > 0
	*/
	template <typename Finite_Integer>
	integer integerLog2(const Finite_Integer& that);

	//! Returns ceil(log_2(that)).
	/*!
	Time complexity:
	O(log(bits(that))),

	Preconditions:
	that > 0
	*/
	template <typename Finite_Integer>
	integer integerCeilLog2(const Finite_Integer& that);

}

#include "pastel/sys/math/logarithm.hpp"

#endif
