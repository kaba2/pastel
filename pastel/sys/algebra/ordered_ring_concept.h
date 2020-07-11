// Description: Ordered ring concept
// Documentation: algebra.txt

#ifndef PASTELSYS_ORDERED_RING_CONCEPT_H
#define PASTELSYS_ORDERED_RING_CONCEPT_H

#include "pastel/sys/algebra/ordered_group_concept.h"
#include "pastel/sys/algebra/ring_concept.h"

namespace Pastel
{

	//! An ordered ring.
	/*! 
	An ordered ring is a ring (X, +, *) such that
	1) (X, +) is an ordered additive group, and
	2) x >= 0 and y >= 0 ==> x * y >= 0, for all x, y in X.
	*/
	template <typename T>
	concept Ordered_Ring_Concept_ =
		Ordered_Additive_Group_Concept<T> &&
		Ring_Concept<T>;

	template <typename T>
	concept Ordered_Ring_Concept =
		Ordered_Ring_Concept_<RemoveCvRef<T>>;

}

#endif
