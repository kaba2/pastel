// Description: Computation of floor(2^w / n)
// Documentation: bit_tricks.txt

#ifndef PASTELSYS_DIVIDE_INFINITY_H
#define PASTELSYS_DIVIDE_INFINITY_H

#include "pastel/sys/mytypes.h"
#include "pastel/sys/integer/integer_concept.h"

namespace Pastel
{

	//! Returns floor(((Integer)Infinity() + 1) / n).
	/*!
	Preconditions:
	n >= 2

	This computation is tricky because inf + 1 = 0
	due to two's complement arithmetic when using
	unsigned integers.
	*/
	template <
		Integer_Concept_ Integer,
		Integer_Concept_ N_Integer
	>
	Integer divideInfinity(const N_Integer& n);

}

#include "pastel/sys/math/divide_infinity.hpp"

#endif
