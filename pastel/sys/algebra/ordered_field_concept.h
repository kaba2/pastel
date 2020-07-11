// Description: Ordered field concept
// Documentation: algebra.txt

#ifndef PASTELSYS_ORDERED_FIELD_CONCEPT_H
#define PASTELSYS_ORDERED_FIELD_CONCEPT_H

#include "pastel/sys/algebra/ordered_ring_concept.h"
#include "pastel/sys/algebra/field_concept.h"

namespace Pastel
{

	//! An ordered field.
	/*! 
	An ordered field is a field which is also
	an ordered ring.
	*/
	template <typename T>
	concept Ordered_Field_Concept_ =
		Ordered_Ring_Concept_<T> &&
		Multiplicative_Group_Concept_<T>;

}

#endif
