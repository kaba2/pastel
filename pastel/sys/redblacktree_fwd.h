#ifndef PASTELSYS_REDBLACKTREE_FWD_H
#define PASTELSYS_REDBLACKTREE_FWD_H

#include "pastel/sys/object_forwarding.h"

namespace Pastel
{

	namespace RedBlackTree_
	{

		template <typename Key, typename Data>
		class Iterator;

		template <typename Key, typename Data>
		class ConstIterator;

	}

	template <typename Settings>
	class RedBlackTree_Fwd
	{
	public:
		typedef typename Settings::Key Key;
		typedef typename Settings::Data Data;
		typedef typename Settings::Compare Compare;

		typedef typename AsClass<Data>::type Data_Class;

		typedef RedBlackTree_::Iterator<Key, Data> Iterator;
		typedef RedBlackTree_::ConstIterator<Key, Data> ConstIterator;
	};

}

#endif
