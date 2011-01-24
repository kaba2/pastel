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
		//! Returns whether 'left' relates to 'right'.
		/*!
		The relation must be a strict total order. That is,
		asymmetric, transitive, and total.
		*/
		template <
			typename Left, 
			typename Right>
		bool operator()(
			const Left& left, 
			const Right& right);
	};

}

#endif
