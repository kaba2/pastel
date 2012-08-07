#ifndef PASTEL_REDBLACKTREE_FWD_H
#define PASTEL_REDBLACKTREE_FWD_H

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

	template <
		typename Key,
		typename Compare,
		typename Data>
	class RedBlackTree_Fwd
	{
	public:
		typedef Key Key_;
		typedef Compare Compare_;
		typedef Data Data_;

		typedef typename AsClass<Data>::type Data_Class;

		typedef RedBlackTree_::Iterator<Key, Data> Iterator;
		typedef RedBlackTree_::ConstIterator<Key, Data> ConstIterator;
	};

}

#endif
