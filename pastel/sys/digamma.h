// Description: Digamma function
// Documentation: math_functions.txt

#ifndef PASTEL_DIGAMMA_H
#define PASTEL_DIGAMMA_H

#include "pastel/sys/mytypes.h"

namespace Pastel
{

	//! Digamma function for integer arguments.
	/*!
	Preconditions:
	n > 0

	digamma(n) = harmonicNumber(n - 1) - constantEulerMascheroni
	*/
	PASTELSYS real64 digammaReal64(integer n);

	//! [Digamma function]
	/*!
	Preconditions :
	n > 0
	*/
	template <typename Real>
	Real digamma(integer n);

}

#include "pastel/sys/digamma.hpp"

#endif
