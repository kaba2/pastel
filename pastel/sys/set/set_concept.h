#ifndef PASTELSYS_SET_CONCEPT_H
#define PASTELSYS_SET_CONCEPT_H

#include "pastel/sys/set/multiset_concept.h"

namespace Pastel
{

	struct Set_Concept
	: Refines<MultiSet_Concept>
	{
	};
	
}

#endif
