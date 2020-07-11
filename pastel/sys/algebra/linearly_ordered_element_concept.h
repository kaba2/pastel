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
	template <typename T>
	concept Linearly_Ordered_Element_Concept_ =
		Ordered_Element_Concept<T>;

	template <typename T>
	concept Linearly_Ordered_Element_Concept =
		Linearly_Ordered_Element_Concept_<RemoveCvRef<T>>;

}

#endif
