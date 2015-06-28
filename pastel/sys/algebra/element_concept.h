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
	struct Element_Concept
	{
		template <typename Type>
		auto requires(Type&& t) -> decltype
		(
			conceptCheck(
				//! Returns whether left == right.
				Concept::convertsTo<bool>(t == t),
				//! Returns whether left != right.
				Concept::convertsTo<bool>(t != t)
			)
		);
	};

}

#endif
