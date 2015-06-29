// Description: Indicator concept

#ifndef PASTELSYS_INDICATOR_CONCEPT_H
#define PASTELSYS_INDICATOR_CONCEPT_H

#include "pastel/sys/mytypes.h"

namespace Pastel
{

	struct Indicator_Concept
	{
		template <
			typename Type,
			typename Element>
		auto requires(Type&& t, Element&& e) -> decltype
		(
			//! Returns whether an element is in the true-set.
			/*!
			The true-set is the set of those elements which the
			indicator maps to true. Similarly for the false-set.
			The false-set is the complement of the true-set.
			*/
			conceptCheck(
				Concept::convertsTo<bool>(addConst(t)(e))
			)
		);
	};

}

#endif
