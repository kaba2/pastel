// Description: Less concept
// Detail: Strict weak order
// Documentation: strict_weak_order.txt

#ifndef PASTELSYS_LESS_CONCEPT_H
#define PASTELSYS_LESS_CONCEPT_H

#include "pastel/sys/predicate/predicate_concept.h"

namespace Pastel
{

	namespace Less_Concept
	{

		//! Strict weak order
		class Less
		: public Predicate_Concept::Predicate
		{
		public:
			//! Returns whether left < right under a strict weak order <.
			/*!
			A strict weak order is an irreflexive and transitive relation
			for which incomparability is transitive.
			*/
			bool operator()(
				const UserDefinedType& left, 
				const UserDefinedType& right) const;
		};

	}

}

#endif
