// Description: Ordered group concept
// Documentation: algebra.txt

#ifndef PASTELSYS_ORDERED_SEMIGROUP_CONCEPT_H
#define PASTELSYS_ORDERED_SEMIGROUP_CONCEPT_H

#include "pastel/sys/algebra/semigroup_concept.h"
#include "pastel/sys/algebra/ordered_element_concept.h"

namespace Pastel
{

	//! An ordered additive semi-group.
	/*! 
	An ordered additive semi-group is
	a semi-group (X, +, 0) which is also
	an ordered set such that
		
		    x <= y ==> c + x <= c + y,
		
	for all c, x, y in X.
	*/
	struct Ordered_Additive_SemiGroup_Concept
	: Refines<
		Additive_SemiGroup_Concept,
		Ordered_Element_Concept>
	{};

}

#endif
