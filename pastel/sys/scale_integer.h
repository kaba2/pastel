// Description: Integer scaling
// Documentation: math_functions.txt

#ifndef PASTELSYS_SCALE_INTEGER_H
#define PASTELSYS_SCALE_INTEGER_H

#include "pastel/sys/mytypes.h"

namespace Pastel
{

	//! Scales an integer from [0, 2^FromBits - 1] to [0, 2^ToBits - 1].
	/*!
	Preconditions:
	FromBits > 0
	ToBits > 0
	ToBits <= 4 * FromBits

	The last requirement can be extended by implementing
	new functions for the larger ranges.

	The computation is effectively:
	round[(number * (2^ToBits - 1)) / (2^(FromBits) - 1)]
	It turns out this can be computed efficiently by using 
	integer shifts and additions alone.
	*/
	template <int FromBits, int ToBits, typename Integer>
	PASTEL_ENABLE_IF_C((ToBits < FromBits), Integer)
		scaleInteger(const Integer& number);

	template <int FromBits, int ToBits, typename Integer>
	PASTEL_ENABLE_IF_C((ToBits == FromBits), Integer)
		scaleInteger(const Integer& number);

	template <int FromBits, int ToBits, typename Integer>
	PASTEL_ENABLE_IF_C((ToBits > FromBits && ToBits <= 2 * FromBits), Integer)
		scaleInteger(const Integer& number);

	template <int FromBits, int ToBits, typename Integer>
	PASTEL_ENABLE_IF_C((ToBits > 2 * FromBits && ToBits <= 3 * FromBits), Integer)
		scaleInteger(const Integer& number);

	template <int FromBits, int ToBits, typename Integer>
	PASTEL_ENABLE_IF_C((ToBits > 3 * FromBits && ToBits <= 4 * FromBits), Integer)
		scaleInteger(const Integer& number);

}

#include "pastel/sys/scale_integer.hpp"

#endif
