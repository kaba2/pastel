// Description: Field concept
// Documentation: algebra.txt

#ifndef PASTELSYS_FIELD_CONCEPT_H
#define PASTELSYS_FIELD_CONCEPT_H

#include "pastel/sys/algebra/ring_concept.h"
#include "pastel/sys/algebra/group_concept.h"

#include "pastel/sys/algebra/native_field.h"

namespace Pastel
{

	//! A field.
	/*!
	A field (X, *, 1, +, 0) is a ring
	such that (X, *, 1) is a commutative group.
	*/
	struct Field_Concept
	: Refines<
		Ring_Concept,
		Multiplicative_Group_Concept>
	{};

}

#endif
