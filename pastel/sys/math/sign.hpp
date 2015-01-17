#ifndef PASTELSYS_SIGN_HPP
#define PASTELSYS_SIGN_HPP

#include "pastel/sys/math/sign.h"

namespace Pastel
{

	template <typename Type>
	integer sign(const Type& that)
	{
		// x>0  x<0   Result
		//  no   no   0 (could also be NaN)
		//  no  yes   -1
		// yes   no   +1
		// yes  yes   Impossible.

		return (integer)(0 < that) - (integer)(that < 0);
	}

}

#endif
