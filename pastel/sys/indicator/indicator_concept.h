// Description: Indicator concept

#ifndef PASTELSYS_INDICATOR_CONCEPT_H
#define PASTELSYS_INDICATOR_CONCEPT_H

#include "pastel/sys/function/function_concept.h"
#include <concepts>

namespace Pastel
{

	//! Returns whether an element is in the true-set.
	/*!
	The true-set is the set of those elements which the
	indicator maps to true. Similarly for the false-set.
	The false-set is the complement of the true-set.
	*/
	template <typename T, typename Element>
	concept Indicator_Concept = 
		Function_Concept<RemoveCvRef<T>, bool, RemoveCvRef<Element>>;

}

#endif
