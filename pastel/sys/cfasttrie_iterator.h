// Description: C-fast trie key-iterator

#ifndef PASTELSYS_CFASTTRIE_ITERATOR_H
#define PASTELSYS_CFASTTRIE_ITERATOR_H

#include "pastel/sys/cfasttrie.h"

#include <boost/iterator/iterator_adaptor.hpp>

#include <type_traits>

namespace Pastel
{

	namespace CFastTrie_
	{

		template <
			typename Iterator,
			typename Key>
		class Key_ConstIterator
			: public boost::iterator_adaptor<
			Key_ConstIterator<Iterator, Key>, 
			Iterator,
			const Key,
			boost::bidirectional_traversal_tag>
		{
		public:
			Key_ConstIterator()
				: Key_ConstIterator::iterator_adaptor_()
			{
			}

			Key_ConstIterator(Iterator that)
				: Key_ConstIterator::iterator_adaptor_(that)
			{
			}

		private:
			friend class boost::iterator_core_access;

			const Key& dereference() const
			{
				return base().key();
			}
		};
	
	}

}

namespace Pastel
{

	namespace CFastTrie_
	{

		template <
			typename Iterator,
			typename Key>
		class Continuous_Iterator
			: public boost::iterator_adaptor<
			Continuous_Iterator<Iterator, Key>, 
			Iterator>
		{
		public:
			Continuous_Iterator()
				: Continuous_Iterator::iterator_adaptor_()
			{
			}

			Continuous_Iterator(const Iterator& that)
				: Continuous_Iterator::iterator_adaptor_(that)
			{
			}

			template <
				typename That_Iterator,
				typename That_Key,
				typename = PASTEL_ENABLE_IF((boost::is_convertible<That_Iterator, Iterator>), void)>
			Continuous_Iterator(
				const Continuous_Iterator<That_Iterator, That_Key>& that)
				: Continuous_Iterator::iterator_adaptor_(that.base())
			{
			}

			const Key& key() const
			{
				return base().key();				
			}

			bool isSentinel() const
			{
				return base().isSentinel();
			}

			bool isGlobalSentinel() const
			{
				return base().right() == base();
			}

			bool equalToChain() const
			{
				return base().data().equalToChain();
			}

		private:
			friend class boost::iterator_core_access;

			void increment()
			{
				Iterator& iter = base_reference();
				++iter;
				if (isSentinel() && !isGlobalSentinel())
				{
					iter = std::next(iter.sentinelData().bundle)->begin().base();
				}
			}

			void decrement()
			{
				Iterator& iter = base_reference();
				--iter;
				if (isSentinel() && !isGlobalSentinel())
				{
					iter = std::prev(iter.sentinelData().bundle)->last().base();
				}
			}
		};
	
	}

}

#endif
