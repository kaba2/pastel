// Description: Harmonic numbers
// Documentation: math_functions.txt

#ifndef PASTELSYS_HARMONIC_H
#define PASTELSYS_HARMONIC_H

#include "pastel/sys/mytypes.h"

namespace Pastel
{

	//! Computes a [harmonic number].
	/*!
	Preconditions:
	n >= 0

	H_0 = 0
	H_n = 1/1 + 1/2 + ... + 1/n
	*/
	template <typename Real>
	Real harmonicNumber(integer n);

}

#include "pastel/sys/harmonic.hpp"

#endif
