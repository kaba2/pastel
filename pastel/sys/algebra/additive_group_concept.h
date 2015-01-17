// Description: Additive group concept
// Documentation: algebra.txt

#ifndef PASTELSYS_ADDITIVE_GROUP_CONCEPT_H
#define PASTELSYS_ADDITIVE_GROUP_CONCEPT_H

#include "pastel/sys/algebra/additive_monoid_concept.h"

namespace Pastel
{

	namespace Additive_Group_Concept
	{

		//! An additive group.
		/*!
		An additive group is an additive monoid (X, +)
		such that for every x in X there exists y in X
		such that x + y = 0. Since the y is unique,
		it is denoted by -x.
		*/
		class Additive_Group
		: public Additive_Monoid_Concept::Additive_Monoid
		{
		public:
			//! Adds -that to the element.
			Additive_Group& operator-=(const Additive_Group& that);

			//! Returns -x for an element x.
			Additive_Group operator-();
		};

		//! Returns left - right.
		Additive_Group operator-(
			const Additive_Group& left,
			const Additive_Group& right);

	}

}

#endif
