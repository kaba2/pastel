// Description: Ordered additive group concept
// Documentation: algebra.txt

#ifndef PASTELSYS_ORDERED_ADDITIVE_SEMIGROUP_CONCEPT_H
#define PASTELSYS_ORDERED_ADDITIVE_SEMIGROUP_CONCEPT_H

#include "pastel/sys/algebra/additive_semigroup_concept.h"
#include "pastel/sys/algebra/ordered_set_concept.h"

namespace Pastel
{

	//! An ordered additive semi-group.
	/*! 
	An ordered additive semi-group is
	a semi-group (X, +) which is also
	an ordered set such that
		
		    x <= y ==> c + x <= c + y,
		
	for all c, x, y in X.
	*/
	struct Ordered_Additive_SemiGroup_Concept
	: Refines<
		Additive_SemiGroup_Concept,
		Ordered_Set_Concept>
	{};

}

#endif
