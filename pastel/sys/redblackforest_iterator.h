// Description: Red-black forest iterator

#ifndef PASTELSYS_REDBLACKFOREST_ITERATOR_H
#define PASTELSYS_REDBLACKFOREST_ITERATOR_H

#include "pastel/sys/redblackforest_fwd.h"

namespace Pastel
{

	namespace RedBlackForest_
	{

		template <typename Base_Iterator>
		class Iterator
			: public Base_Iterator
		{
		public:
			// We use inheritance, because we wish to inherit
			// all the functionality of the red-black tree
			// iterator, such as checking the color of a node.

			//! Forwards all constructors to the tree-iterator.
			/*!
			Time complexity: O(1)
			Exception safety: nothrow
			*/
			template <typename... Type>
			Iterator(Type&&... that)
			: Base_Iterator(std::forward<Type>(that)...)
			{
			}

			//! Returns whether this is the end-node of the forest.
			/*!
			Time complexity: O(1)
			Exception safety: nothrow
			*/
			bool isForestEnd() const
			{
				return isSentinel() &&
					sentinelData().isForestEnd();
			}

			//! Moves to the next element.
			/*!
			Time complexity: 
			O(1) amortized + O(k)
			where
			k is the number of skipped empty trees.

			Exception safety: nothrow
			*/
			Iterator& operator++()
			{
				Base_Iterator& iter = *this;
				++iter;
				
				// Skip all end-nodes, except for the
				// forest end-node.
				while (isSentinel() && !isForestEnd())
				{
					iter = std::next(iter.sentinelData().tree())->begin();
				}

				return *this;
			}

			//! Moves to the next element.
			/*!
			This is a convenience function which calls
			operator++().
			*/
			Iterator operator++(int)
			{
				Iterator result = *this;
				++(*this);
				return result;
			}

			//! Moves to the previous element.
			/*!
			Time complexity: 
			O(1) amortized + O(k)
			where
			k is the number of skipped empty trees.

			Exception safety: nothrow
			*/
			Iterator& operator--()
			{
				Base_Iterator& iter = *this;
				--iter;
				
				while (isSentinel())
				{
					iter = std::prev(iter.sentinelData().tree())->last();
				}

				if (isForestEnd())
				{
					iter = std::next(iter.sentinelData().tree())->begin();
				}

				return *this;
			}

			//! Moves to the previous element.
			/*!
			This is a convenience function which calls
			operator--().
			*/
			Iterator operator--(int)
			{
				Iterator result = *this;
				--(*this);
				return result;
			}
		};
	
	}

}

#endif
