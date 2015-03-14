// Description: Additive monoid concept
// Documentation: algebra.txt

#ifndef PASTELSYS_ADDITIVE_MONOID_CONCEPT_H
#define PASTELSYS_ADDITIVE_MONOID_CONCEPT_H

#include "pastel/sys/algebra/additive_semigroup_concept.h"

namespace Pastel
{

	//! An additive monoid.
	/*!
	An additive monoid is an additive semi-group (X, +)
	such that there exist 0 in X such that 
		
		    x + 0 = x, for all x in X.
	*/
	struct Additive_Monoid_Concept
	: Refines<Additive_SemiGroup_Concept>
	{
		template <typename Type>
		auto requires(Type&& t) -> decltype
		(
			conceptCheck(
				// Returns whether t == 0.
				Concept::convertsTo<bool>(zero(t))
			)
		);
	};

}

#endif
