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
		const NoDeduction<Type>& a,
		const NoDeduction<Type>& b,
		Type& min, Type& max);

	//! Computes the minimum and maximum.

	template <typename Type>
		void minMax(
		const NoDeduction<Type>& a,
		const NoDeduction<Type>& b,
		const NoDeduction<Type>& c,
		Type& min, Type& max);

}

#include "pastel/sys/minmax.hpp"

#endif
