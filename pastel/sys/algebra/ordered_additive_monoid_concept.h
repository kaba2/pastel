// Description: Ordered additive monoid concept
// Documentation: algebra.txt

#ifndef PASTELSYS_ORDERED_ADDITIVE_MONOID_CONCEPT_H
#define PASTELSYS_ORDERED_ADDITIVE_MONOID_CONCEPT_H

#include "pastel/sys/algebra/ordered_additive_semigroup_concept.h"
#include "pastel/sys/algebra/additive_monoid_concept.h"

#include "pastel/sys/algebra/native_ordered_additive_monoid.h"

namespace Pastel 
{

	//! An ordered additive monoid.
	/*! 
	An ordered additive monoid is an additive monoid
	which is also an ordered additive semigroup.
	*/
	struct Ordered_Additive_Monoid_Concept
	: Refines<
		Ordered_Additive_SemiGroup_Concept,
		Additive_Monoid_Concept>
	{
		template <typename Type>
		auto requires(Type&& t) -> decltype
		(
			conceptCheck(
				//! Returns whether 'that' > 0.
				Concept::convertsTo<bool>(positive(t)),
				//! Returns whether 'that' < 0.
				Concept::convertsTo<bool>(negative(t)),
				//! Returns the absolute value of 'that'.
				Concept::convertsTo<Type>(abs(t))
			)
		);
	};

}

#endif
