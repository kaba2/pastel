// Description: RbtPolicy concept
// Documentation: rbtpolicies.txt

#ifndef PASTEL_RBTPOLICY_CONCEPT_H
#define PASTEL_RBTPOLICY_CONCEPT_H

#include "pastel/sys/mytypes.h"

namespace Pastel
{

	namespace RbtPolicy_Concept
	{

		class RbtPolicy
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
			Time complexity: O(1).
			*/
			void swap(RbtPolicy& that);

			//! Updates the hierarchical data in a node.
			/*!
			This function updates the hierarchical data for the
			subtree pointed to by 'iter' under the assumption that 
			the child subtrees are up to date. It is called by the
			insert() and erase() functions of the RedBlackTree
			whenever the structure of the subtree has changed.
			Note that the complexities of the insert() and erase()
			functions are multiplied by the complexity of this 
			function. Thus, for example, to retain O(log n) 
			complexity for those functions, this function must 
			perform in O(1) (which is usually the case when the
			hierarchical data is recursively defined).
			*/
			template <typename Node_Iterator>
			void updateHierarchical(const Node_Iterator& iter);
		};

	}

}

#endif
