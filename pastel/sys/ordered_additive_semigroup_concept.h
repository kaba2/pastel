// Description: Ordered additive group concept
// Documentation: algebra.txt

#ifndef PASTELSYS_ORDERED_ADDITIVE_SEMIGROUP_CONCEPT_H
#define PASTELSYS_ORDERED_ADDITIVE_SEMIGROUP_CONCEPT_H

#include "pastel/sys/additive_semigroup_concept.h"

namespace Pastel
{

	namespace Ordered_Additive_SemiGroup_Concept
	{

		// An ordered additive semi-group is
		// a semi-group (X, +) together with 
		// a relation <= subset X^2 such that
		//
		//     x <= y ==> c + x <= c + y,
		//
		// for all c, x, y in X.

		class Ordered_Additive_SemiGroup
		: public Additive_SemiGroup_Concept::Additive_SemiGroup
		{
		};

		//! Returns whether left < right.
		bool operator<(
			const Ordered_Additive_SemiGroup& left,
			const Ordered_Additive_SemiGroup& right);

		//! Returns whether left > right.
		bool operator>(
			const Ordered_Additive_SemiGroup& left,
			const Ordered_Additive_SemiGroup& right);

		//! Returns whether left <= right.
		bool operator<=(
			const Ordered_Additive_SemiGroup& left,
			const Ordered_Additive_SemiGroup& right);
		
		//! Returns whether left >= right.
		bool operator>=(
			const Ordered_Additive_SemiGroup& left,
			const Ordered_Additive_SemiGroup& right);

	}

}

#endif
