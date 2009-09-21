// Description: Min-max computations
// Documentation: numbers.txt

#ifndef PASTEL_MINMAX_H
#define PASTEL_MINMAX_H

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

#include "pastel/math/minmax.hpp"

#endif
