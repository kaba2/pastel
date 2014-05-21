// Description: Predicate concept
// Documentation: predicates.txt

#ifndef PASTELSYS_PREDICATE_CONCEPT_H
#define PASTELSYS_PREDICATE_CONCEPT_H

#include "pastel/sys/algorithm_concept.h"

namespace Pastel
{

	namespace Predicate_Concept
	{
		
		class Predicate
		{
		public:
			//! Returns whether 'left' relates to 'right'.
			bool operator()(
				const UserDefinedType& left, 
				const UserDefinedType& right);
		};

	}

}

#endif
