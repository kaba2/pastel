// Description: Input concept
// Documentation: inputs.txt

#ifndef PASTELSYS_INPUT_CONCEPT_H
#define PASTELSYS_INPUT_CONCEPT_H

#include "pastel/sys/mytypes.h"

namespace Pastel
{

	namespace Input_Concept
	{

		class Input
		{
		public:
			// The type of the elements.
			typedef UserDefinedType Type;

			//! Returns the number of elements left.
			/*!
			This number is only a hint;
			the implementation must be able to deal with
			either a smaller or a larger amounts of elements.
			This can be used, for example, to pre-allocate memory
			for an array. A negative number specifies that 
			the number is not known. 
			*/
			integer nHint() const;

			//! Returns whether there are elements left.
			bool empty() const;

			//! Returns the next element.
			/*!
			Preconditions:
			!empty()
			*/
			Type operator()();
		};

	}

}

#endif
