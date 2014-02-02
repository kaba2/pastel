// Description: Ordered additive monoid concept
// Documentation: algebra.txt

#ifndef PASTELSYS_ORDERED_ADDITIVE_MONOID_CONCEPT_H
#define PASTELSYS_ORDERED_ADDITIVE_MONOID_CONCEPT_H

#include "pastel/sys/ordered_additive_semigroup_concept.h"
#include "pastel/sys/additive_monoid_concept.h"

namespace Pastel
{

	namespace Ordered_Additive_Monoid_Concept
	{

		// An ordered additive monoid is an additive monoid
		// which is also an ordered additive semigroup.

		class Ordered_Additive_Monoid
		: public Ordered_Additive_SemiGroup_Concept::Ordered_Additive_SemiGroup
		, public Additive_Monoid_Concept::Additive_Monoid
		{
		};

		//! Returns whether 'that' > 0.
		bool positive(
			const Ordered_Additive_Monoid& that);

		//! Returns whether 'that' < 0.
		bool negative(
			const Ordered_Additive_Monoid& that);

		//! Returns the absolute value of 'that'.
		Ordered_Additive_Monoid abs(
			const Ordered_Additive_Monoid& that);

	}

}

#endif
