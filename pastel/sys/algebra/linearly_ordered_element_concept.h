// Description: Linearly-ordered element concept
// Documentation: algebra.txt

#ifndef PASTELSYS_LINEARLY_ORDERED_ELEMENT_CONCEPT_H
#define PASTELSYS_LINEARLY_ORDERED_ELEMENT_CONCEPT_H

#include "pastel/sys/algebra/ordered_element_concept.h"

namespace Pastel
{

	//! A linearly-ordered set-element.
	/*! 
	A linearly-ordered set is an ordered-set
	whose order relation is total.
	*/
	struct Linearly_Ordered_Element_Concept
		: Refines<Ordered_Element_Concept>
	{};

	template <typename T>
	concept Linearly_Ordered_Element_Concept__ =
		Ordered_Element_Concept_<T>;

	template <typename T>
	concept Linearly_Ordered_Element_Concept_ =
		Linearly_Ordered_Element_Concept__<RemoveCvRef<T>>;

}

#endif
