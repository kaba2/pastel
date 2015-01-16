// Description: Input concept
// Documentation: input.txt

#ifndef PASTELSYS_INPUT_CONCEPT_H
#define PASTELSYS_INPUT_CONCEPT_H

#include "pastel/sys/mytypes.h"

#include <type_traits>

namespace Pastel
{

	namespace Input_Concept
	{

		class Input
		{
		public:
			//! Returns the number of elements left.
			/*!
			This number is only a hint;
			the implementation must be able to deal with
			either a smaller or a larger amounts of elements.
			This can be used, for example, to pre-allocate memory
			for an array. A zero specifies that the number is 
			not known; it conveys zero information.

			Must return a non-negative number.
			*/
			integer nHint() const;

			//! Returns whether there are elements left.
			bool empty() const;

			//! Returns the current element.
			/*!
			Preconditions:
			!empty()
			*/
			UserDefinedType get() const;

			//! Retrieves the next element.
			/*!
			!empty()
			*/
			void pop();
		};

	}

}

#include "pastel/sys/input_return.h"
#include "pastel/sys/input_value.h"

#endif
