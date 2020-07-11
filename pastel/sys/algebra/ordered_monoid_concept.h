// Description: Ordered additive monoid concept
// Documentation: algebra.txt

#ifndef PASTELSYS_ORDERED_MONOID_CONCEPT_H
#define PASTELSYS_ORDERED_MONOID_CONCEPT_H

#include "pastel/sys/algebra/ordered_semigroup_concept.h"
#include "pastel/sys/algebra/monoid_concept.h"

#include "pastel/sys/algebra/native_ordered_monoid.h"

namespace Pastel 
{

	//! An ordered additive monoid.
	/*! 
	An ordered monoid (X, +, 0) is an additive monoid (X, +, 0)
	which is also an ordered additive semigroup.
	*/
	template <typename T>
	concept Ordered_Additive_Monoid_Concept__ =
		Ordered_Additive_SemiGroup_Concept_<T> && 
		Additive_Monoid_Concept_<T> && 
		requires(T t) {
		//! Returns whether 'that' > 0.
		{positive(t)} -> std::convertible_to<bool>;
		//! Returns whether 'that' < 0.
		{negative(t)} -> std::convertible_to<bool>;
		//! Returns the absolute value of 'that'.
		{abs(t)} -> std::convertible_to<T>;
	};

	template <typename T>
	concept Ordered_Additive_Monoid_Concept_ =
		Ordered_Additive_Monoid_Concept__<RemoveCvRef<T>>;

}

#endif
