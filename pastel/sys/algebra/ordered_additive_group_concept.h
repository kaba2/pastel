// Description: Ordered additive group concept
// Documentation: algebra.txt

#ifndef PASTELSYS_ORDERED_ADDITIVE_GROUP_CONCEPT_H
#define PASTELSYS_ORDERED_ADDITIVE_GROUP_CONCEPT_H

#include "pastel/sys/algebra/ordered_additive_monoid_concept.h"
#include "pastel/sys/algebra/additive_group_concept.h"

namespace Pastel
{

	//! An ordered additive group.
	/*!
	An ordered additive group is
	an ordered additive monoid which
	is also a group.
	*/
	struct Ordered_Additive_Group_Concept
	: Refines<
		Ordered_Additive_Monoid_Concept,
		Additive_Group_Concept>
	{
	};

}

#endif
