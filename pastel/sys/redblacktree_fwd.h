#ifndef PASTELSYS_REDBLACKTREE_FWD_H
#define PASTELSYS_REDBLACKTREE_FWD_H

#include "pastel/sys/object_forwarding.h"
#include "pastel/sys/range.h"

#include <type_traits>

namespace Pastel
{

	enum
	{
		RedBlackTree_Dereference_Default,
		RedBlackTree_Dereference_Key,
		RedBlackTree_Dereference_Data
	};

	namespace RedBlackTree_
	{

		template <typename, typename, typename, bool>
		class Iterator;

		template <typename, typename>
		class Node;

	}

	template <typename Settings>
	class RedBlackTree_Fwd
	{
	public:
		using Key = typename Settings::Key;
		using Data = typename Settings::Data;
		using Compare = typename Settings::Compare;
		using Data_Class = Class<Data>;
		using Node = RedBlackTree_::Node<Key, Data_Class>;

		using Key_Iterator = 
			RedBlackTree_::Iterator<Node*, Key, Data_Class, false>;
		using Key_ConstIterator = 
			RedBlackTree_::Iterator<const Node*, Key, Data_Class, false>;
		using Key_Range = 
			boost::iterator_range<Key_Iterator>;
		using Key_ConstRange = 
			boost::iterator_range<Key_ConstIterator>;

		using Data_Iterator = 
			RedBlackTree_::Iterator<Node*, Key, Data_Class, true>;
		using Data_ConstIterator = 
			RedBlackTree_::Iterator<const Node*, Key, Data_Class, true>;
		using Data_Range = 
			boost::iterator_range<Data_Iterator>;
		using Data_ConstRange = 
			boost::iterator_range<Data_ConstIterator>;

		static const bool DereferenceToData =
			(Settings::DereferenceType == RedBlackTree_Dereference_Data) ||
			(Settings::DereferenceType == RedBlackTree_Dereference_Default &&
			!std::is_same<Data, void>::value);

		using Iterator = 
			RedBlackTree_::Iterator<Node*, Key, Data_Class, DereferenceToData>;
		using ConstIterator = 
			RedBlackTree_::Iterator<const Node*, Key, Data_Class, DereferenceToData>;
		using Range = 
			boost::iterator_range<Iterator>;
		using ConstRange = 
			boost::iterator_range<ConstIterator>;
	};

}

#endif
