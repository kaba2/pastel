// Description: Linearly-ordered set concept
// Documentation: algebra.txt

#ifndef PASTELSYS_LINEARLY_ORDERED_SET_CONCEPT_H
#define PASTELSYS_LINEARLY_ORDERED_SET_CONCEPT_H

#include "pastel/sys/ordered_set_concept.h"

namespace Pastel
{

	namespace Linearly_Ordered_Set_Concept
	{

		//! A linearly-ordered set.
		/*! 
		A linearly-ordered set is an ordered-set
		whose order relation is total.
		*/
		class Linearly_Ordered_Set
		: public Ordered_Set_Concept::Ordered_Set
		{
		};

	}

}

#endif
