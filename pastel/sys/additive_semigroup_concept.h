// Description: Additive semi-group concept
// Documentation: algebra.txt

#ifndef PASTELSYS_ADDITIVE_SEMIGROUP_CONCEPT_H
#define PASTELSYS_ADDITIVE_SEMIGROUP_CONCEPT_H

#include "pastel/sys/comparable_concept.h"

namespace Pastel
{

	namespace Additive_SemiGroup_Concept
	{

		// An additive semi-group is a pair (X, +),
		// where X is a set and + : X^2 --> X is 
		// commutative and associative.

		class Additive_SemiGroup
		: public Comparable_Concept::Comparable
		{
		public:
			//! Adds 'that' to the element.
			Additive_SemiGroup& operator+=(
				const Additive_SemiGroup& that);
		};

		//! Returns left + right.
		Additive_SemiGroup operator+(
			const Additive_SemiGroup& left,
			const Additive_SemiGroup& right);

	}

}

#endif
