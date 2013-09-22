// Description: Indicator concept

#ifndef PASTELSYS_INDICATOR_CONCEPT_H
#define PASTELSYS_INDICATOR_CONCEPT_H

#include "pastel/sys/mytypes.h"

namespace Pastel
{

	namespace Indicator_Concept
	{

		class Indicator
		{
		public:
			//! Returns whether an element is in the true-set.
			/*!
			The true-set is the set of those elements which the
			indicator maps to true. Similarly for the false-set.
			The false-set is the complement of the true-set.
			*/
			template <typename Type>
			bool operator()(const Type& element) const;
		};

	}

}

#endif
