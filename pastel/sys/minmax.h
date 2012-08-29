// Description: Min-max computations
// Documentation: math_functions.txt

#ifndef PASTELSYS_MINMAX_H
#define PASTELSYS_MINMAX_H

#include "pastel/sys/mytypes.h"

namespace Pastel
{

	//! Computes the minimum and maximum.

	template <typename Type>
		void minMax(
		const PASTEL_NO_DEDUCTION(Type)& a,
		const PASTEL_NO_DEDUCTION(Type)& b,
		Type& min, Type& max);

	//! Computes the minimum and maximum.

	template <typename Type>
		void minMax(
		const PASTEL_NO_DEDUCTION(Type)& a,
		const PASTEL_NO_DEDUCTION(Type)& b,
		const PASTEL_NO_DEDUCTION(Type)& c,
		Type& min, Type& max);

}

#include "pastel/sys/minmax.hpp"

#endif
