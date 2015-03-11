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
			integer nHint() const;

			//! Returns whether there are elements left.
			bool empty() const;

			//! Returns the current element.
			/*!
			Preconditions:
			!empty()
			*/
			UserDefinedType get() const;

			//! Drops the current element off from the input.
			/*!
			Preconditions:
			!empty()
			*/
			void pop();
		};

		class Sized_Input
		: public Input
		{
		public:
			//! Returns the exact number of elements left.
			integer n() const;
		};

		class Indexed_Input
		: public Sized_Input
		{
		public:
			//! Returns the i:th next element.
			/*!
			Preconditions:
			0 <= i < n()
			*/
			UserDefinedType operator[](integer i) const;
		};

	}

}

#include "pastel/sys/input/input_return.h"
#include "pastel/sys/input/input_value.h"

#endif
