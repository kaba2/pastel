// Description: Ordered ring concept
// Documentation: algebra.txt

#ifndef PASTELSYS_ORDERED_RING_CONCEPT_H
#define PASTELSYS_ORDERED_RING_CONCEPT_H

#include "pastel/sys/ordered_additive_group_concept.h"
#include "pastel/sys/ring_concept.h"

namespace Pastel
{

	namespace Ordered_Ring_Concept
	{

		//! An ordered ring.
		/*! 
		An ordered ring is a ring (X, +, *) such that
		1) (X, +) is an ordered additive group, and
		2) x >= 0 and y >= 0 ==> x * y >= 0, for all x, y in X.
		*/
		class Ordered_Ring
		: public Ordered_Additive_Group_Concept::Ordered_Additive_Group
		, public Ring_Concept::Ring
		{
		};

	}

}

#endif
