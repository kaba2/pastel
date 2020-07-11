// Description: Ordered additive group concept
// Documentation: algebra.txt

#ifndef PASTELSYS_ORDERED_GROUP_CONCEPT_H
#define PASTELSYS_ORDERED_GROUP_CONCEPT_H

#include "pastel/sys/algebra/ordered_monoid_concept.h"
#include "pastel/sys/algebra/group_concept.h"

namespace Pastel
{

	//! An ordered additive group.
	/*!
	An ordered group (X, +, 0) is an ordered
	monoid (X, +, 0) which is also a group.
	*/
	template <typename T>
	concept Ordered_Additive_Group_Concept_ =
		Ordered_Additive_Monoid_Concept<T> && 
		Additive_Group_Concept<T>;

	template <typename T>
	concept Ordered_Additive_Group_Concept =
		Ordered_Additive_Group_Concept_<RemoveCvRef<T>>;

}

#endif
