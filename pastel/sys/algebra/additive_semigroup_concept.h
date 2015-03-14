// Description: Additive semi-group concept
// Documentation: algebra.txt

#ifndef PASTELSYS_ADDITIVE_SEMIGROUP_CONCEPT_H
#define PASTELSYS_ADDITIVE_SEMIGROUP_CONCEPT_H

#include "pastel/sys/algebra/set_concept.h"

namespace Pastel
{

	//! An additive semi-group.
	/*!
	An additive semi-group is a pair (X, +),
	where X is a set and + : X^2 --> X is 
	commutative and associative.
	*/
	struct Additive_SemiGroup_Concept
	: Refines<Set_Concept>
	{
		template <typename Type>
		auto requires(Type&& t) -> decltype
		(
			conceptCheck(
				//! Adds 'that' to the element.
				Concept::hasType<Type&>(t += t),
				//! Returns left + right.
				Concept::convertsTo<Type>(t + t)
			)
		);
	};

}

#endif
