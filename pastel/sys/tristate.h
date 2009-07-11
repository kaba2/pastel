#ifndef PASTEL_TRISTATE_H
#define PASTEL_TRISTATE_H

namespace Pastel
{

	//! A triple state variable.

	/*!
	Sometimes it is required to answer a question with not only
	true or false, but also with 'both' or 'neither' or
	some other choice. The sign of a variable is a TriState:
	the sign might be negative, positive, or the variable
	might be zero, that is, not negative nor positive.
	The logic functions are generalized to these tristates,
	with false being 0, true being 1, and values not true
	or false being 0.5. The used t-norm is max.
	*/

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
