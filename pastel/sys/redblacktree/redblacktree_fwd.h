// Description: Red-black tree types

#ifndef PASTELSYS_REDBLACKTREE_FWD_H
#define PASTELSYS_REDBLACKTREE_FWD_H

#include "pastel/sys/range.h"

#include <type_traits>
#include <memory>

namespace Pastel
{

	namespace RedBlackTree_
	{

		template <typename, typename, bool>
		class Iterator;

		class Node;

		template <typename>
		class Propagation_Node;

		template <typename>
		class Sentinel_Node;
	
		template <typename>
		class Data_Node;

		template <typename>
		class Key_Node;

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

		static constexpr bool MultipleKeys =
			Settings::MultipleKeys;

		static constexpr bool UserDataInSentinelNodes =
			Settings::UserDataInSentinelNodes;

		static constexpr bool UserDataInSentinelNodes_ =
			UserDataInSentinelNodes;

		using Key_ = Key;
		using Data_ = Data;
		using Propagation_ = Propagation;
		using SentinelData_ = SentinelData;

		static constexpr bool DereferenceToData =
			!std::is_same<Data, Empty>::value;

		class Node_Settings;
		using Propagation_Node = RedBlackTree_::Propagation_Node<Node_Settings>;
		using Data_Node = RedBlackTree_::Data_Node<Node_Settings>;

		class Node_Settings
		{
		public:
			PASTEL_FWD(Key);
			using Propagation = Propagation_;
			using Data = Data_;
			using SentinelData = SentinelData_;
			static constexpr bool UserDataInSentinelNodes =
				UserDataInSentinelNodes_;
			using EndBase = typename std::conditional<
				UserDataInSentinelNodes,
				Data_Node,
				Propagation_Node>::type;
		};

		using Node = RedBlackTree_::Node;
		using Sentinel_Node = RedBlackTree_::Sentinel_Node<Node_Settings>;
		using SentinelPtr = std::shared_ptr<Sentinel_Node>;
		using Key_Node = RedBlackTree_::Key_Node<Node_Settings>;

		template <
			typename NodePtr, 
			bool DereferenceToData>
		class Range_
		: public ranges::subrange<RedBlackTree_::Iterator<
			NodePtr, Node_Settings, DereferenceToData>>
		{
		public:
			using Base = ranges::subrange<RedBlackTree_::Iterator<
				NodePtr, Node_Settings, DereferenceToData>>;

			//! Forwards all constructors to the underlying range.
			/*!
			Time complexity: O(1)
			Exception safety: nothrow
			*/
			template <typename... Type>
			explicit Range_(Type&&... that)
				: Base(std::forward<Type>(that)...)
			{
			}

			template <typename That_NodePtr, bool That_DereferenceToData>
			Range_(const Range_<That_NodePtr, That_DereferenceToData>& that)
			: Base(that)
			{
			}

			using Key_Range = Range_<NodePtr, false>;
			using Data_Range = Range_<NodePtr, true>;

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
			Range_<Node*, DereferenceToData>;
		using ConstRange = 
			Range_<const Node*, DereferenceToData>;

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

namespace Pastel
{

	template <
		typename Key_ = Empty, 
		typename Data_ = Empty,
		typename Less_ = LessThan,
		typename Propagation_ = Empty,
		typename SentinelData_ = Empty,
		bool MultipleKeys_ = false,
		bool UserDataInSentinelNodes_ = false>
	class RedBlackTree_Set_Settings
	{
	public:
		using Key = Key_;
		using Data = Data_;
		using Less = Less_;
		using Propagation = Propagation_;
		using SentinelData = SentinelData_;
		static constexpr bool MultipleKeys = MultipleKeys_;
		static constexpr bool UserDataInSentinelNodes = UserDataInSentinelNodes_;
	};

}

// FIX: We could do the below much clearer by forwarding a parameter pack
// instead. Unfortunately, that triggers a bug in Visual Studio 2013 SP2.
// Fix this after the bug is resolved.

namespace Pastel
{

	// Map
	
	template <
		typename Key = Empty, 
		typename Data = Empty,
		typename Less = LessThan,
		typename Propagation = Empty,
		typename SentinelData = Empty,
		bool UserDataInSentinelNodes = false>
	using RedBlackTree_Set_Fwd = 
		RedBlackTree_Fwd<RedBlackTree_Set_Settings<
			Key, Data, Less, Propagation, SentinelData, false,
			UserDataInSentinelNodes>>;

	// Multi-map

	template <
		typename Key = Empty, 
		typename Data = Empty,
		typename Less = LessThan,
		typename Propagation = Empty,
		typename SentinelData = Empty,
		bool UserDataInSentinelNodes = false>
	using RedBlackTree_MultiSet_Fwd = 
		RedBlackTree_Fwd<RedBlackTree_Set_Settings<
			Key, Data, Less, Propagation, SentinelData, true,
			UserDataInSentinelNodes>>;

}

#endif
