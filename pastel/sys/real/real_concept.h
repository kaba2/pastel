// Description: Real concept
// Documentation: reals.txt

#ifndef PASTELSYS_REAL_CONCEPT_H
#define PASTELSYS_REAL_CONCEPT_H

#include "pastel/sys/real/real_ring_concept.h"
#include "pastel/sys/algebra/group_concept.h"

namespace Pastel
{

	//! A dreal number.
	template <typename T>
	concept Real_Concept_ = 
		Real_Ring_Concept<T> && 
		Multiplicative_Group_Concept<T>;

	template <typename T>
	concept Real_Concept =
		Real_Concept_<RemoveCvRef<T>>;

	using Real_Archetype = dreal;

}

#endif
