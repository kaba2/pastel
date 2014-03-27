// Description: Red-black tree types

#ifndef PASTELSYS_REDBLACKTREE_FWD_H
#define PASTELSYS_REDBLACKTREE_FWD_H

#include "pastel/sys/object_forwarding.h"
#include "pastel/sys/range.h"

#include <type_traits>
#include <memory>

namespace Pastel
{

	namespace RedBlackTree_
	{

		template <typename, typename, bool>
		class Iterator;

		template <typename>
		class Node_Base;

		template <typename>
		class Data_Node;
		
		template <typename>
		class Node;

	}

	template <typename Settings>
	class RedBlackTree_Fwd
	{
	public:
		using Fwd = Settings;
		
		PASTEL_FWD(Key);
		PASTEL_FWD(Propagation);
		PASTEL_FWD(Data);
		PASTEL_FWD(Less);

		PASTEL_CONSTEXPR bool MultipleKeys =
			Settings::MultipleKeys;

		PASTEL_CONSTEXPR bool StoreSentinelPropagation =
			Settings::StoreSentinelPropagation;

		using Data_Class = Class<Data>;
		using Propagation_Class = Class<Propagation>;

		PASTEL_CONSTEXPR bool DereferenceToData =
			!std::is_same<Data, void>::value;

		class Node_Settings
		{
		public:
			PASTEL_FWD(Key);
			using Propagation_Class = Propagation_Class;
			using Data_Class = Data_Class;
			PASTEL_CONSTEXPR bool StoreSentinelPropagation =
				Settings::StoreSentinelPropagation;
		};

		using Node = RedBlackTree_::Node<Node_Settings>;
		using Sentinel = typename std::conditional<
			StoreSentinelPropagation,
			RedBlackTree_::Data_Node<Node_Settings>,
			RedBlackTree_::Node_Base<Node_Settings>>::type;
		using EndPtr = Sentinel*;
		using BottomPtr = std::shared_ptr<Sentinel>;

		using Key_Iterator = 
			RedBlackTree_::Iterator<Node*, Node_Settings, false>;
		using Key_ConstIterator = 
			RedBlackTree_::Iterator<const Node*, Node_Settings, false>;
		using Key_Range = 
			boost::iterator_range<Key_Iterator>;
		using Key_ConstRange = 
			boost::iterator_range<Key_ConstIterator>;

		using Data_Iterator = 
			RedBlackTree_::Iterator<Node*, Node_Settings, true>;
		using Data_ConstIterator = 
			RedBlackTree_::Iterator<const Node*, Node_Settings, true>;
		using Data_Range = 
			boost::iterator_range<Data_Iterator>;
		using Data_ConstRange = 
			boost::iterator_range<Data_ConstIterator>;

		using Iterator = 
			RedBlackTree_::Iterator<Node*, Node_Settings, DereferenceToData>;
		using ConstIterator = 
			RedBlackTree_::Iterator<const Node*, Node_Settings, DereferenceToData>;
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
