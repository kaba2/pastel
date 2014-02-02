// Description: Additive monoid concept
// Documentation: algebra.txt

#ifndef PASTELSYS_ADDITIVE_MONOID_CONCEPT_H
#define PASTELSYS_ADDITIVE_MONOID_CONCEPT_H

#include "pastel/sys/additive_semigroup_concept.h"

namespace Pastel
{

	namespace Additive_Monoid_Concept
	{

		// An additive monoid is an additive semi-group (X, +)
		// such that there exist 0 in X such that 
		// x + 0 = x, for all x in X.

		class Additive_Monoid
		: public Additive_SemiGroup_Concept::Additive_SemiGroup
		{
		};

		//! Returns whether that == 0.
		bool zero(const Additive_Monoid& that);

	}

}

#endif
