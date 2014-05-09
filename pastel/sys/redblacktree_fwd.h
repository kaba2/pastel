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
		class Base_Node;

		template <typename>
		class Propagation_Node;
		
		template <typename>
		class Sentinel_Node;

		template <typename>
		class Node;

	}

	template <typename, template <typename> class>
	class RedBlackTree;

	template <typename Settings>
	class RedBlackTree_Fwd
	{
	public:
		using Fwd = Settings;
		
		PASTEL_FWD(Key);
		PASTEL_FWD(Propagation);
		PASTEL_FWD(Data);
		PASTEL_FWD(SentinelData);
		PASTEL_FWD(Less);

		PASTEL_CONSTEXPR bool MultipleKeys =
			Settings::MultipleKeys;

		using Data_Class = Class<Data>;
		using Propagation_Class = Class<Propagation>;
		using SentinelData_Class = Class<SentinelData>;

		PASTEL_CONSTEXPR bool DereferenceToData =
			!std::is_same<Data, void>::value;

		class Node_Settings
		{
		public:
			PASTEL_FWD(Key);
			using Propagation_Class = Propagation_Class;
			using Data_Class = Data_Class;
			using SentinelData_Class = SentinelData_Class;
		};

		using Node = RedBlackTree_::Node<Node_Settings>;
		using Sentinel_Node = RedBlackTree_::Sentinel_Node<Node_Settings>;
		using SentinelPtr = std::shared_ptr<Sentinel_Node>;

		template <
			typename NodePtr, 
			typename Node_Settings, 
			bool DereferenceToData>
		class Range_
		: public boost::iterator_range<RedBlackTree_::Iterator<
			NodePtr, Node_Settings, DereferenceToData>>
		{
		public:
			using Base = boost::iterator_range<RedBlackTree_::Iterator<
				NodePtr, Node_Settings, DereferenceToData>>;

			//! Forwards all constructors to the underlying range.
			/*!
			Time complexity: O(1)
			Exception safety: nothrow
			*/
			template <typename... Type>
			Range_(Type&&... that)
				: Base(std::forward<Type>(that)...)
			{
			}

			using Key_Range = Range_<NodePtr, Node_Settings, false>;
			using Data_Range = Range_<NodePtr, Node_Settings, true>;

			Key_Range dereferenceKey() const
			{
				return *this;
			}

			Data_Range dereferenceData() const
			{
				return *this;
			}
		};

		using Iterator = 
			RedBlackTree_::Iterator<Node*, Node_Settings, DereferenceToData>;
		using ConstIterator = 
			RedBlackTree_::Iterator<const Node*, Node_Settings, DereferenceToData>;
		using Range = 
			Range_<Node*, Node_Settings, DereferenceToData>;
		using ConstRange = 
			Range_<const Node*, Node_Settings, DereferenceToData>;

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
