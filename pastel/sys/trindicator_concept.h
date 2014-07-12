// Description: Three-state indicator concept
// Documentation: generic_programming.txt

#ifndef PASTELSYS_TRINDICATOR_CONCEPT_H
#define PASTELSYS_TRINDICATOR_CONCEPT_H

#include "pastel/sys/mytypes.h"

namespace Pastel
{

	namespace Trindicator_Concept
	{

		class Trindicator
		{
		public:
			//! Returns whether 'that' is part of the set.
			/*!
			+ = part of the set,
			0 = fuzzy boundary of the set, and
			- = not part of the set.
			*/
			integer operator()(const UserDefinedType& that) const;
		};

	}

}

#endif
