// Description: Ordered element concept
// Documentation: algebra.txt

#ifndef PASTELSYS_ORDERED_ELEMENT_CONCEPT_H
#define PASTELSYS_ORDERED_ELEMENT_CONCEPT_H

#include "pastel/sys/algebra/element_concept.h"

namespace Pastel
{

	//! An ordered-set element.
	/*! 
	An ordered-set is a set X together with
	a partial order < in X. The other relations
	>, <=, and >= are defined in the obvious
	way.
	*/
	struct Ordered_Element_Concept
		: Refines<Element_Concept>
	{
		template <typename Type>
		auto requires(Type&& t) -> decltype
		(
			conceptCheck(
				//! Returns whether left < right.
				Concept::convertsTo<bool>(t < t),
				//! Returns whether left > right.
				Concept::convertsTo<bool>(t > t),
				//! Returns whether left <= right.
				Concept::convertsTo<bool>(t <= t),
				//! Returns whether left >= right.
				Concept::convertsTo<bool>(t >= t)
			)
		);
	};

}

#endif