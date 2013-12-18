// Description: Logarithms
// Documentation: math_functions.txt

#ifndef PASTELSYS_LOGARITHM_H
#define PASTELSYS_LOGARITHM_H

#include "pastel/sys/integer_concept.h"

namespace Pastel
{

	//! Computes floor(log_2(x)).
	/*!
	Preconditions:
	x >= 1
	*/
	template <typename Real>
	integer floorLog2(
		PASTEL_NO_DEDUCTION(Real) x);

	//! Computes the [base-2 logarithm] of x.
	/*!
	Preconditions:
	x >= 0
	*/
	template <typename Real>
	Real log2(
		const PASTEL_NO_DEDUCTION(Real)& x);

	//! Returns floor(log_2(that)).
	/*!
	Time complexity:
	O(log(log(that + 4))),

	Preconditions:
	that > 0
	*/
	integer integerLog2(integer that);

	//! Returns ceil(log_2(that)).
	/*!
	Time complexity:
	O(log(log(that + 4))),

	Preconditions:
	that > 0
	*/
	integer integerCeilLog2(integer that);

}

#include "pastel/sys/logarithm.hpp"

#endif
