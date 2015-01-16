// Description: Fuzzy 3-state logic
// Documentation: math_functions.txt

#ifndef PASTELSYS_TRISTATE_H
#define PASTELSYS_TRISTATE_H

namespace Pastel
{

	//! A triple state variable.
	enum class TriState : integer
	{
		False = 0,
		Negative = 0,
		True = 1,
		Positive = 1,
		Maybe = 2,
		Both = 2,
		Neither = 2,
		Zero = 2
	};

	void swap(TriState& left, TriState& right);

	TriState triNot(TriState that);
	TriState triMix(TriState left, TriState right);
	TriState triEqual(TriState left, TriState right);
	TriState triNotEqual(TriState left, TriState right);
	TriState triAnd(TriState left, TriState right);
	TriState triOr(TriState left, TriState right);
	TriState triXor(TriState left, TriState right);

	template <typename Type>
	TriState triLess(
		const Type& left, const Type& right);

}

#include "pastel/sys/tristate.hpp"

#endif
