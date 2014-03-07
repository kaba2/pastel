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
			typename Base_Iterator,
			typename Key>
		class Key_ConstIterator
			: public boost::iterator_adaptor<
			Key_ConstIterator<Base_Iterator, Key>, 
			Base_Iterator,
			const Key,
			boost::bidirectional_traversal_tag>
		{
		public:
			Key_ConstIterator()
				: Key_ConstIterator::iterator_adaptor_()
			{
			}

			Key_ConstIterator(Base_Iterator that)
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

#endif
