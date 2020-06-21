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

	template <typename T>
	concept Indexed_Input_Concept__ =
		Sized_Input_Concept_<T> && 
		requires(T t) {
			//! Returns the i:th next element.
			/*!
			Preconditions:
			0 <= i < n()
			*/
			{t[(integer)0]} -> std::same_as<decltype(t.get())>;
	};

	template <typename T>
	concept Indexed_Input_Concept_ =
		Indexed_Input_Concept__<RemoveCvRef<T>>;

}

#endif
