// Description: Real concept
// Documentation: reals.txt

#ifndef PASTELSYS_REAL_CONCEPT_H
#define PASTELSYS_REAL_CONCEPT_H

#include "pastel/sys/real/real_ring_concept.h"
#include "pastel/sys/algebra/group_concept.h"

namespace Pastel
{

	//! A real number.
	struct Real_Concept
	: Refines<
		Real_Ring_Concept,
		Multiplicative_Group_Concept>
	{};

	using Real_Archetype = real;

}

#endif
