// Description: Sign function
// Documentation: math_functions.txt

#ifndef PASTELSYS_SIGN_H
#define PASTELSYS_SIGN_H

#include "pastel/sys/real/real_concept.h"

namespace Pastel
{

	//! Signum function.
	/*!
	returns:
	-1, if negative,
	 0, if zero (or NaN), or
	+1, if positive.
	*/
	template <typename Real>
	integer sign(const Real& that)
	{
		// x>0  x<0   Result
		//  no   no   0 (could also be NaN)
		//  no  yes   -1
		// yes   no   +1
		// yes  yes   Impossible.

		return (integer)positive(that) - (integer)negative(that);
	}

}

#endif
