// Description: Powers
// Documentation: math_functions.txt

#ifndef PASTELSYS_POWERS_H
#define PASTELSYS_POWERS_H

namespace Pastel
{

	//! Squares the given variable.
	template <typename Type>
	Type square(const Type& that);

	//! Returns 2^k.
	/*!
	Preconditions:
	k >= 0
	*/
	template <typename Integer>
	Integer powerOfTwo(integer k);

}

#include "pastel/sys/math/powers.hpp"

#endif
