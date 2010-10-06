#ifndef PASTEL_RBTPOLICT_CONCEPT_H
#define PASTEL_RBTPOLICT_CONCEPT_H

#include "pastel/sys/mytypes.h"

namespace Pastel
{

	class RbtPolicy_Concept
	{
	public:
		//! The type of the value data stored in nodes.
		/*!
		If value data is not needed, define this EmptyClass, 
		so that no memory is spent on the values.
		*/
		typedef UserDefinedType ValueType;

		//! Swaps two policies.
		/*!
		This operation should be O(1) to preserve
		complexity guarantees.
		*/
		void swap(RbtPolicy_Concept& that);

		//! Updates the hierarchical data in a node.
		template <typename Iterator>
		void updateHierarchicalData(const Iterator& iter);
	};

}

#endif
