// Description: Order relation algorithm concept

#ifndef PASTEL_ORDERRELATIONALGORITHM_CONCEPT_H
#define PASTEL_ORDERRELATIONALGORITHM_CONCEPT_H

#include "pastel/sys/relationalgorithm_concept.h"

namespace Pastel
{

	class OrderRelationAlgorithm_Concept
		: public RelationAlgorithm_Concept
	{
	public:
		//! Returns whether 'a' relates to 'b'.
		/*!
		The relation must be a strict total order. That is,
		asymmetric, transitive, and total.
		*/
		template <typename A, typename B>
		bool operator()(const A& a, const B& b);
	};

}

#endif
