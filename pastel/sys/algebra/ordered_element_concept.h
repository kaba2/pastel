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
	template <typename T>
	concept Ordered_Element_Concept__ =
		Element_Concept_<T> && 
		requires(T t) {
			//! Returns whether left < right.
			{t < t} -> std::convertible_to<bool>;
			//! Returns whether left > right.
			{t > t} -> std::convertible_to<bool>;
			//! Returns whether left <= right.
			{t <= t} -> std::convertible_to<bool>;
			//! Returns whether left >= right.
			{t >= t} -> std::convertible_to<bool>;
	};

	template <typename T>
	concept Ordered_Element_Concept_ =
		Ordered_Element_Concept__<RemoveCvRef<T>>;

}

#endif
