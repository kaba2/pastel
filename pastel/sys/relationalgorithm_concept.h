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
		//! Returns whether 'a' relates to 'b'.
		template <typename A, typename B>
		bool operator()(const A& a, const B& b);
	};

}

#endif
