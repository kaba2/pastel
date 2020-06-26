// Description: Real concept
// Documentation: reals.txt

#ifndef PASTELSYS_REAL_CONCEPT_H
#define PASTELSYS_REAL_CONCEPT_H

#include "pastel/sys/real/real_ring_concept.h"
#include "pastel/sys/algebra/group_concept.h"

namespace Pastel
{

	//! A dreal number.
	struct Real_Concept
	: Refines<
		Real_Ring_Concept,
		Multiplicative_Group_Concept>
	{};

	template <typename T>
	concept Real_Concept__ = 
		Real_Ring_Concept_<T> && 
		Multiplicative_Group_Concept_<T>;

	template <typename T>
	concept Real_Concept_ =
		Real_Concept__<RemoveCvRef<T>>;

	using Real_Archetype = dreal;

}

#endif
