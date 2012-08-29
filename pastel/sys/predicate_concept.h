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
			template <
				typename Left, 
				typename Right>
			bool operator()(
				const Left& left, 
				const Right& right);
		};

	}

}

#endif
