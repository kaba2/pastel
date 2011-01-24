// Description: Relation algorithm concept

#ifndef PASTEL_RELATIONALGORITHM_CONCEPT_H
#define PASTEL_RELATIONALGORITHM_CONCEPT_H

#include "pastel/sys/algorithm_concept.h"

namespace Pastel
{

	class RelationAlgorithm_Concept
		: public Algorithm_Concept
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

#endif
