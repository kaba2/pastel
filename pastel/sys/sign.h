// Description: Sign function
// Documentation: math_functions.txt

#ifndef PASTELSYS_SIGN_H
#define PASTELSYS_SIGN_H

#include "pastel/sys/mytypes.h"

namespace Pastel
{

	//! Signum function.
	/*!
	returns:
	-1, if negative,
	 0, if zero (or NaN), or
	+1, if positive.
	*/
	template <typename Type>
	integer sign(const Type& that);

}

#include "pastel/sys/sign.hpp"

#endif
