// Description: Set concept
// Documentation: algebra.txt

#ifndef PASTELSYS_SET_CONCEPT_H
#define PASTELSYS_SET_CONCEPT_H

#include "pastel/sys/concept.h"

namespace Pastel
{

	//! A set.
	/*!
	A set is any type whose objects can be
	compared with == and !=.
	*/
	struct Set_Concept
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
