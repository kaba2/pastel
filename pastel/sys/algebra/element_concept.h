// Description: Element concept
// Documentation: algebra.txt

#ifndef PASTELSYS_ELEMENT_CONCEPT_H
#define PASTELSYS_ELEMENT_CONCEPT_H

#include "pastel/sys/concept/concept.h"

namespace Pastel
{

	//! A set element.
	/*!
	A set element is any type whose objects can 
	be compared with == and !=.
	*/
	template <typename T>
	concept Element_Concept_ = requires(T t) {
		//! Returns whether left == right.
		{t == t} -> std::convertible_to<bool>;
		//! Returns whether left != right.
		{t != t} -> std::convertible_to<bool>;
	};

	template <typename T>
	concept Element_Concept = 
		Element_Concept_<RemoveCvRef<T>>;

}

#endif
