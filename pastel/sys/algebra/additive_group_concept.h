// Description: Additive group concept
// Documentation: algebra.txt

#ifndef PASTELSYS_ADDITIVE_GROUP_CONCEPT_H
#define PASTELSYS_ADDITIVE_GROUP_CONCEPT_H

#include "pastel/sys/algebra/additive_monoid_concept.h"

namespace Pastel
{

	//! An additive group.
	/*!
	An additive group is an additive monoid (X, +)
	such that for every x in X there exists y in X
	such that x + y = 0. Since the y is unique,
	it is denoted by -x.
	*/
	struct Additive_Group_Concept
	: Refines<Additive_Monoid_Concept>
	{
		template <typename Type>
		auto requires(Type&& t) -> decltype
		(
			conceptCheck(
				//! Adds -that to the element.
				Concept::hasType<Type&>(t -= t),
				//! Returns left - right.
				Concept::convertsTo<Type>(t - t),
				//! Returns -x for an element x.
				Concept::convertsTo<Type>(-t)
			)
		);
	};

}

#endif
