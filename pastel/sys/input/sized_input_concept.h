// Description: Sized input concept

#ifndef PASTELSYS_SIZED_INPUT_CONCEPT_H
#define PASTELSYS_SIZED_INPUT_CONCEPT_H

#include "pastel/sys/input/input_concept.h"

namespace Pastel
{

	struct Sized_Input_Concept
	: Refines<Input_Concept>
	{
		template <typename Type>
		auto requires(Type&& t) -> decltype
		(
			//! Returns the exact number of elements left.
			Concept::convertsTo<integer>(t.n())
		);
	};

}

#endif