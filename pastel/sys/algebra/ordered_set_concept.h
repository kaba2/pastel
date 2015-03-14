// Description: Ordered set concept
// Documentation: algebra.txt

#ifndef PASTELSYS_ORDERED_SET_CONCEPT_H
#define PASTELSYS_ORDERED_SET_CONCEPT_H

#include "pastel/sys/algebra/set_concept.h"

namespace Pastel
{

	//! An ordered-set.
	/*! 
	An ordered-set is a set X together with
	a partial order < in X. The other relations
	>, <=, and >= are defined in the obvious
	way.
	*/
	struct Ordered_Set_Concept
		: Refines<Set_Concept>
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
