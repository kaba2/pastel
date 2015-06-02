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
	struct Ordered_Additive_Group_Concept
	: Refines<
		Ordered_Additive_Monoid_Concept,
		Additive_Group_Concept>
	{
	};

}

#endif