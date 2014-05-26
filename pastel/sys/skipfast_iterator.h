// Description: Skip-fast trie iterator

#ifndef PASTELSYS_SKIPFAST_ITERATOR_H
#define PASTELSYS_SKIPFAST_ITERATOR_H

#include "pastel/sys/skipfast.h"
#include "pastel/sys/indicator_downfilter.h"

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
			PASTEL_FWD(Chain_ConstIterator);
			PASTEL_FWD(Chain_Iterator_Local);
			PASTEL_FWD(Chain_ConstIterator_Local);

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

			//! Returns the chain of the element.
			/*!
			Time complexity: O(?)
			Exception safety: nothrow
			*/
			Chain_Iterator findChain() const
			{
				Base_Iterator iter = *this;
				while (!iter.isSentinel())
				{
					iter = iter.parent();
				}
				return iter.sentinelData().chain;
			}

			//! Moves to the next element.
			/*!
			Time complexity: 
			O(1) amortized + O(log(k))
			where
			k is the number of skipped empty chains.

			Exception safety: nothrow
			*/
			template <bool Right = true>
			Iterator next() const
			{
				PASTEL_CONSTEXPR integer Step = Right ? 1 : -1;

				auto onlyNonEmpty = indicatorDownFilter(
					[](const Chain_ConstIterator& chain) {return !chain->elementSet_.empty(); },
					[](const Chain_ConstIterator& chain) {return chain.propagation().nonEmpty;}
				);

				// Go to the next element.
				Base_Iterator iter = std::next((Base_Iterator&)*this, Step);

				if (iter.isSentinel())
				{
					// There are no more elements in the
					// current chain.

					// Find the current chain.
					Chain_Iterator_Local chain = 
						iter.sentinelData().chain;

					// Go to the next non-empty chain.
					chain = chain.next<Right>(onlyNonEmpty);

					if (chain.isSentinel())
					{
						// There are no more non-empty chains 
						// in the current chain-group.

						// Find the current chain-group.
						auto group = Chain_ConstIterator(chain).findTree();

						while (!std::next(group, 2 * Step).isEnd() &&
							chain.isSentinel())
						{
							// Go to the next chain-group.
							group = std::next(group, Step);

							if (std::next(group, 2 * Step).isEnd())
							{
								// This is the end-group, so it does not contain
								// non-empty chains. Return the end-chain.
								chain = group->begin();
								ASSERT(!chain.isSentinel());
								ASSERT(chain->elementSet_.empty());
								break;
							}

							// Pick the extremum chain of the chain-group.
							chain = group->end().next<Right>(onlyNonEmpty);
						}
					}

					if (!chain.isSentinel())
					{
						if (!chain->elementSet_.empty())
						{
							// Pick the extremum element of the chain.
							iter = chain->elementSet_.extremum(!Right);
						}
						else
						{
							iter = chain->elementSet_.end();
						}
					}
				}
				
				return Iterator(iter);
			}

			Iterator& operator++()
			{
				*this = next<true>();
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
				*this = next<false>();
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
