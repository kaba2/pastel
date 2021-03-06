// Description: Lexicographic order
// Documentation: math_functions.txt

#ifndef PASTELSYS_LEXICOGRAPHIC_H
#define PASTELSYS_LEXICOGRAPHIC_H

#include "pastel/sys/mytypes.h"
#include "pastel/sys/vector.h"

namespace Pastel
{

	//! Returns whether left comes before right in lexicographic order.
	template <typename Type>
	bool lexicographicLess(
		const NoDeduction<Type>& leftPrimary,
		const NoDeduction<Type>& leftSecondary,
		const NoDeduction<Type>& rightPrimary,
		const NoDeduction<Type>& rightSecondary);

	//! Returns whether left comes before right in lexicographic order.
	template <typename Real, int N>
	bool lexicographicLess(
		const Vector<Real, N>& left,
		const Vector<Real, N>& right);

}

#include "pastel/sys/predicate/lexicographic.hpp"

#endif
