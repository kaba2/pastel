// Description: Computation of floor(2^w / n)
// Documentation: bit_tricks.txt

#ifndef PASTELSYS_DIVIDE_INFINITY_H
#define PASTELSYS_DIVIDE_INFINITY_H

#include "pastel/sys/mytypes.h"

#include <type_traits>

namespace Pastel
{

	//! Returns floor((infinity<uinteger>() + 1) / n).
	/*!
	Preconditions:
	n >= 2

	This computation is tricky because inf + 1 = 0
	due to two's complement arithmetic when using
	unsigned integers.
	*/
	template <typename Type, EnableIf<std::is_unsigned<Type>> = 0>
	Type divideInfinity(Type n);

}

#include "pastel/sys/math/divide_infinity.hpp"

#endif
