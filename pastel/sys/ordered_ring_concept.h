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

		class Ordered_Ring
		: public Ordered_Additive_Group_Concept::Ordered_Additive_Group
		, public Ring_Concept::Ring
		{
		};

	}

}

#endif
