// Description: Clamping
// Documentation: math_functions.txt

#ifndef PASTELSYS_CLAMP_H
#define PASTELSYS_CLAMP_H

namespace Pastel
{

	//! Clamps the variable to a given range.
	/*!
	[Clamping]

	Preconditions:
	xMin <= xMax
	*/
	template <typename Type>
	Type clamp(
		const Type& x,
		const PASTEL_NO_DEDUCTION(Type)& xMin,
		const PASTEL_NO_DEDUCTION(Type)& xMax);

}

#include "pastel/sys/clamp.hpp"

#endif
