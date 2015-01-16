// Description: Ordered set concept
// Documentation: algebra.txt

#ifndef PASTELSYS_ORDERED_SET_CONCEPT_H
#define PASTELSYS_ORDERED_SET_CONCEPT_H

#include "pastel/sys/set_concept.h"

namespace Pastel
{

	namespace Ordered_Set_Concept
	{

		//! An ordered-set.
		/*! 
		An ordered-set is a set X together with
		a partial order < in X. The other relations
		>, <=, and >= are defined in the obvious
		way.
		*/
		class Ordered_Set
		: public Set_Concept::Set
		{
		};

		//! Returns whether left < right.
		bool operator<(
			const Ordered_Set& left,
			const Ordered_Set& right);

		//! Returns whether left > right.
		bool operator>(
			const Ordered_Set& left,
			const Ordered_Set& right);

		//! Returns whether left <= right.
		bool operator<=(
			const Ordered_Set& left,
			const Ordered_Set& right);
		
		//! Returns whether left >= right.
		bool operator>=(
			const Ordered_Set& left,
			const Ordered_Set& right);

	}

}

#endif
