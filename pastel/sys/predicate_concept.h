// Description: Predicate concept

#ifndef PASTEL_PREDICATE_CONCEPT_H
#define PASTEL_PREDICATE_CONCEPT_H

#include "pastel/sys/algorithm_concept.h"

namespace Pastel
{

	namespace Predicate_Concept
	{
		
		class Predicate
			: public Algorithm_Concept::Algorithm
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
