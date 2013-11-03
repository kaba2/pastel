// Description: Powers
// Documentation: math_functions.txt

#ifndef PASTELSYS_POWERS_H
#define PASTELSYS_POWERS_H

namespace Pastel
{

	//! Squares the given variable.
	template <typename Type>
	Type square(const Type& that);

	//! Returns 2^i.
	/*!
	Preconditions:
	i >= 0
	*/
	integer powerOfTwo(integer i);

}

#include "pastel/sys/powers.hpp"

#endif
