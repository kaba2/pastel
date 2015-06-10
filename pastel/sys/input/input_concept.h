// Description: Input concept
// Documentation: input.txt

#ifndef PASTELSYS_INPUT_CONCEPT_H
#define PASTELSYS_INPUT_CONCEPT_H

#include "pastel/sys/mytypes.h"

#include <type_traits>

namespace Pastel
{

	struct Input_Concept
	{
		template <typename Type>
		auto requires(Type&& t) -> decltype
		(
			conceptCheck(
				//! Returns an approximation of the number of elements left.
				/*!
				This number is only a hint;
				the implementation must be able to deal with
				either a smaller or a larger amounts of elements.
				This can be used, for example, to pre-allocate memory
				for an array. A zero specifies that the number is 
				not known; it conveys zero information.

				Must return a non-negative number.
				*/
				Concept::convertsTo<integer>(addConst(t).nHint()),

				//! Returns whether there are elements left.
				Concept::convertsTo<bool>(addConst(t).empty()),

				//! Returns the current element.
				/*!
				Preconditions:
				!empty()
				*/
				(addConst(t).get(), 0),

				//! Drops the current element off from the input.
				/*!
				Preconditions:
				!empty()
				*/
				(t.pop(), 0)
			)
		);
	};

}

#include "pastel/sys/input/input_return.h"
#include "pastel/sys/input/input_value.h"

#endif
