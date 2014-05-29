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

			//! Constructs an empty iterator.
			/*!
			Time complexity: O(1)
			Exception safety: nothrow
			*/
			Iterator()
			: Base_Iterator()
			{
			}

			//! Constructs from a tree iterator.
			/*!
			Time complexity: O(1)
			Exception safety: nothrow
			*/
			Iterator(const Base_Iterator& that)
			: Base_Iterator(that)
			{
			}

			Iterator(const Iterator& that) = default;

			template <
				typename That_BaseIterator,
				typename = PASTEL_ENABLE_IF(
					(std::is_convertible<That_BaseIterator, Base_Iterator>), void)>
			Iterator(const Iterator<That_BaseIterator>& that)
				: Base_Iterator(that)
			{
			}

			decltype(((Base_Iterator*)0)->sentinelData().tree())
				findTree() const
			{
				Base_Iterator iter = *this;
				while (!iter.isSentinel())
				{
					iter = iter.parent();
				}
				return iter.sentinelData().tree();
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
				
				if (isSentinel() && !isForestEnd())
				{
					// Skip empty trees.
					auto tree = iter.sentinelData().tree();
					ASSERT(!tree.isEnd());

					do 
					{
						++tree;
					} while (!tree.isEnd() && tree->empty());
					
					iter = tree->begin();
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
