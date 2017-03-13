// Description: Indexed input concept

#ifndef PASTELSYS_INDEXED_INPUT_CONCEPT_H
#define PASTELSYS_INDEXED_INPUT_CONCEPT_H

#include "pastel/sys/input/sized_input_concept.h"

namespace Pastel
{

	struct Indexed_Input_Concept
	: Refines<Sized_Input_Concept>
	{
		template <typename Type>
		auto requires_(Type&& t) -> decltype
		(
			//! Returns the i:th next element.
			/*!
			Preconditions:
			0 <= i < n()
			*/
			Concept::sameTypes(t[(integer)0], t.get())
		);
	};

}

#endif
