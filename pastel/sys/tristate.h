// Description: Fuzzy 3-state logic
// Documentation: math_functions.txt

#ifndef PASTEL_TRISTATE_H
#define PASTEL_TRISTATE_H

namespace Pastel
{

	//! A triple state variable.

	class TriState
	{
	public:
		enum Enum
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
	};

	void swap(TriState::Enum& left, TriState::Enum& right);

	TriState::Enum triNot(TriState::Enum that);
	TriState::Enum triMix(TriState::Enum left, TriState::Enum right);
	TriState::Enum triEqual(TriState::Enum left, TriState::Enum right);
	TriState::Enum triNotEqual(TriState::Enum left, TriState::Enum right);
	TriState::Enum triAnd(TriState::Enum left, TriState::Enum right);
	TriState::Enum triOr(TriState::Enum left, TriState::Enum right);
	TriState::Enum triXor(TriState::Enum left, TriState::Enum right);

	template <typename Type>
	TriState::Enum triLess(
		const Type& left, const Type& right);

}

#include "pastel/sys/tristate.hpp"

#endif
