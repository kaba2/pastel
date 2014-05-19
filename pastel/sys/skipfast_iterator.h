// Description: Skip-fast trie iterator

#ifndef PASTELSYS_SKIPFAST_ITERATOR_H
#define PASTELSYS_SKIPFAST_ITERATOR_H

#include "pastel/sys/skipfast.h"

#include <boost/iterator/iterator_adaptor.hpp>

#include <type_traits>

namespace Pastel
{

	namespace SkipFast_
	{

		template <
			typename Settings, 
			typename Base_Iterator>
		class Iterator
			: public Base_Iterator
		{
		public:
			using Fwd = SkipFast_Fwd<Settings>;
			PASTEL_FWD(Chain_Iterator);

			//! Forwards all constructors to the base-iterator.
			/*!
			Time complexity: O(1)
			Exception safety: nothrow
			*/
			template <typename... Type>
			Iterator(Type&&... that)
			: Base_Iterator(std::forward<Type>(that)...)
			{
			}

			template <typename That_BaseIterator>
			Iterator(const Iterator<Settings, That_BaseIterator>& that)
				: Base_Iterator(that)
			{
			}

			//! Moves to the next element.
			/*!
			Time complexity: 
			O(1) amortized + O(k)
			where
			k is the number of skipped empty trees.

			Exception safety: nothrow
			*/
			template <integer Step>
			Iterator& increment()
			{
				Base_Iterator& iter = *this;

				// Go to the next element.
				iter = std::next(iter, Step);

				if (iter.isSentinel())
				{
					// There are no more elements in the
					// current chain.
					auto chain = iter.sentinelData().chain;

					// Go to the next chain.
					chain = std::next(chain, Step);

					if (chain.isSentinel())
					{
						// There are no more chains the current
						// chain-group.

						Chain_Iterator globalChain = chain;

						// Find the current chain-group.
						auto group = globalChain.findTree();

						// Go to the next chain-group.
						group = std::next(group, Step);
						if (group->cend().sentinelData().type == GroupType::Empty)
						{
							// The next chain-group contains only
							// empty chains. Skip it.
							group = std::next(group, Step);

						}

						// Pick the extremum chain of the chain-group.
						chain = std::next(group->end(), Step);
					}

					// Pick the extremum element of the chain.
					iter = std::next(chain->elementSet_.end(), Step);
				}
				
				return *this;
			}

			Iterator& operator++()
			{
				return increment<1>();
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
				return increment<-1>();
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
