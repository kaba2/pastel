// Description: Types for the red-black tree

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

		template <typename, typename, bool>
		class Node_Base;

		template <typename, typename, bool>
		class Data_Node;
		
		template <typename, typename, bool>
		class Node;

	}

	template <typename Settings>
	class RedBlackTree_Fwd
	{
	public:
		using Key = typename Settings::Key;
		using Data = typename Settings::Data;
		using Compare = typename Settings::Compare;

		PASTEL_CONSTEXPR bool DereferenceToData =
			(Settings::DereferenceType == RedBlackTree_Dereference_Data) ||
			(Settings::DereferenceType == RedBlackTree_Dereference_Default &&
			!std::is_same<Data, void>::value);

		PASTEL_CONSTEXPR bool MultipleKeys =
			Settings::MultipleKeys;

		PASTEL_CONSTEXPR bool UseSentinelData =
			Settings::UseSentinelData;

		using Data_Class = Class<Data>;
		using Node = RedBlackTree_::Node<Key, Data_Class, UseSentinelData>;
		using Sentinel = typename std::conditional<
			UseSentinelData,
			RedBlackTree_::Data_Node<Key, Data_Class, UseSentinelData>,
			RedBlackTree_::Node_Base<Key, Data_Class, UseSentinelData>>::type;

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

		using Iterator = 
			RedBlackTree_::Iterator<
				Node*, Key, 
				Data_Class, DereferenceToData>;
		using ConstIterator = 
			RedBlackTree_::Iterator<
				const Node*, Key, 
				Data_Class, DereferenceToData>;
		using Range = 
			boost::iterator_range<Iterator>;
		using ConstRange = 
			boost::iterator_range<ConstIterator>;

		using Insert_Return =
			typename std::conditional<MultipleKeys,
			Iterator,
			std::pair<Iterator, bool >>::type;

		struct FindEqual_Return
		{
			//! The top-most element equivalent to the key.
			ConstIterator equal;

			//! An element greater than the key.
			/*!
			This is the least element greater than the key,
			subject to being an ancestor of 'equal'.
			*/
			ConstIterator upper;
		};

		struct FindInsert_Return
		{
			//! The element under which the key should be inserted.
			/*!
			The position is chosen subject only to the binary search
			property. Simply linking the key under the returned
			node preserves the binary-search property, but
			usually breaks the red-black invariants.
			*/
			ConstIterator parent;
			
			//! Whether the insertion should be to the right child.
			bool right;

			//! An element greater than the key.
			/*!
			This is the least element greater than the key,
			subject to being an ancestor of 'parent'.
			*/
			ConstIterator upper;
		};
	};

}

#endif
