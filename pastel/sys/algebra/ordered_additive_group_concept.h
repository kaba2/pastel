// Description: Ordered additive group concept
// Documentation: algebra.txt

#ifndef PASTELSYS_ORDERED_ADDITIVE_GROUP_CONCEPT_H
#define PASTELSYS_ORDERED_ADDITIVE_GROUP_CONCEPT_H

#include "pastel/sys/algebra/ordered_additive_monoid_concept.h"
#include "pastel/sys/algebra/additive_group_concept.h"

namespace Pastel
{

	namespace Ordered_Additive_Group_Concept
	{

		//! An ordered additive group.
		/*!
		An ordered additive group is
		an ordered additive monoid which
		is also a group.
		*/
		class Ordered_Additive_Group
		: public Ordered_Additive_Monoid_Concept::Ordered_Additive_Monoid
		, public Additive_Group_Concept::Additive_Group
		{
		};

	}

}

#endif
