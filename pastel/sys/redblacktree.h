// Description: Red-black tree

#ifndef PASTELSYS_REDBLACKTREE_H
#define PASTELSYS_REDBLACKTREE_H

#include "pastel/sys/redblacktree_fwd.h"
#include "pastel/sys/redblacktree_concepts.h"
#include "pastel/sys/redblacktree_node.h"
#include "pastel/sys/redblacktree_iterator.h"

namespace Pastel
{

	template <typename Settings>
	using Empty_RedBlackTree_Customization =
		RedBlackTree_Concepts::Customization<Settings>;

	//! Red-black tree
	template <
		typename Settings_,
		typename Customization = Empty_RedBlackTree_Customization<Settings_>>
	class RedBlackTree
		: public Customization
	{
	public:
		using Settings = Settings_;
		using Fwd = RedBlackTree_Fwd<Settings>;

		PASTEL_FWD(Key);
		PASTEL_FWD(Data);
		PASTEL_FWD(Compare);

		PASTEL_FWD(Iterator);
		PASTEL_FWD(ConstIterator);
		PASTEL_FWD(Range);
		PASTEL_FWD(ConstRange);

		PASTEL_FWD(Key_Iterator);
		PASTEL_FWD(Key_ConstIterator);
		PASTEL_FWD(Key_Range);
		PASTEL_FWD(Key_ConstRange);

		PASTEL_FWD(Data_Iterator);
		PASTEL_FWD(Data_ConstIterator);
		PASTEL_FWD(Data_Range);
		PASTEL_FWD(Data_ConstRange);

		PASTEL_FWD(Data_Class);

		using iterator = Iterator;
		using const_iterator = ConstIterator;

		//! Constructs an empty tree.
		/*!
		Exception safety: strong
		Time complexity: O(1)
		*/
		explicit RedBlackTree(
			Key sentinelKey = Key(), 
			Data_Class sentinelData = Data_Class());

		//! Copy-constructs from another tree.
		/*!
		Exception safety: strong
		Time complexity: O(that.size()) * insert()
		*/
		RedBlackTree(const RedBlackTree& that);

		//! Move-constructs from another tree.
		/*!
		Exception safety: nothrow
		Time complexity: O(1)
		*/
		RedBlackTree(RedBlackTree&& that);

		//! Replaces this tree with a copy of another tree.
		/*!
		Exception safety: strong
		Time complexity: move/copy-construction
		*/
		RedBlackTree& operator=(RedBlackTree that);

		//! Destructs the tree.
		/*!
		Exception safety: nothrow
		Time complexity: clear()
		*/
		~RedBlackTree();

		//! Swaps two trees.
		/*!
		Exception safety: nothrow
		Time complexity: O(1)
		*/
		void swap(RedBlackTree& that);

		//! Removes all elements from the tree.
		/*!
		Exception safety: nothrow
		Time complexity: O(size()) + onClear()
		*/
		void clear();

		//! Returns the number of elements in the tree.
		/*!
		Exception safety: nothrow
		Time complexity: O(1)
		*/
		integer size() const;

		//! Returns true if the tree is empty.
		/*!
		Exception safety: nothrow
		Time complexity: O(1)
		*/
		bool empty() const;

		//! Inserts an element into the tree.
		/*!
		Exception safety: strong + onInsert()
		Time complexity: O(log n) * updateHieararchicalData() + onInsert()

		Here f(n) is the time spent computing the
		user-specified hierarchical data (usually O(1)).
		*/
		Iterator insert(
			Key key, 
			Data_Class data = Data_Class());

		//! Inserts elements into the tree.
		/*!
		Exception safety: basic
		Time complexity: insert() * std::distance(begin, end)
		*/
		template <typename Key_ConstIterator_>
		void insertMany(
			Key_ConstIterator_ begin,
			Key_ConstIterator_ end);

		//! Removes an element from the tree.
		/*!
		Exception safety: nothrow
		Time complexity: O(log n) * updateHierarchicalData() + onErase()
		*/
		Iterator erase(const ConstIterator& that);

		//! Removes an element from the tree.
		/*!
		This is a convenience function that calls
		erase(find(key)).
		*/
		Iterator erase(const Key& key);

		//! Searches for a node with the given key.
		/*!
		See the documentation for the const version.
		*/
		Iterator find(const Key& key);
		ConstIterator find(const Key& key) const;

		//! Returns the iterator to the smallest element.
		PASTEL_ITERATOR_FUNCTIONS(begin, minimum PASTEL_CALL_BRACKETS);

		//! Returns the iterator to the one-past-greatest element.
		PASTEL_ITERATOR_FUNCTIONS(end, sentinel_);

		//! Returns an iterator range.
		PASTEL_RANGE_FUNCTIONS(range, begin, end);

		//! Returns the iterator to the smallest element.
		PASTEL_ITERATOR_FUNCTIONS_PREFIX(Key_, keyBegin, minimum PASTEL_CALL_BRACKETS);

		//! Returns the iterator to the one-past-greatest element.
		PASTEL_ITERATOR_FUNCTIONS_PREFIX(Key_, keyEnd, sentinel_);

		//! Returns an iterator range.
		PASTEL_RANGE_FUNCTIONS_PREFIX(Key_, keyRange, keyBegin, keyEnd);

		//! Returns the iterator to the smallest element.
		PASTEL_ITERATOR_FUNCTIONS_PREFIX(Data_, dataBegin, minimum PASTEL_CALL_BRACKETS);

		//! Returns the iterator to the one-past-greatest element.
		PASTEL_ITERATOR_FUNCTIONS_PREFIX(Data_, dataEnd, sentinel_);

		//! Returns an iterator range.
		PASTEL_RANGE_FUNCTIONS_PREFIX(Data_, dataRange, dataBegin, dataEnd);

		//! Returns the iterator to the greatest element.
		PASTEL_ITERATOR_FUNCTIONS(last, maximum PASTEL_CALL_BRACKETS);

		//! Returns the iterator to the root element.
		/*!
		See the documentation for the const version.
		*/
		PASTEL_ITERATOR_FUNCTIONS(root, root_);

	private:
		enum
		{
			Left = 0,
			Right = 1
		};

		typedef RedBlackTree_::Node<Key, Data_Class> Node;

		//! Allocates the sentinel node.
		/*!
		Exception safety: strong
		Time complexity: constant
		*/
		void allocateSentinel(
			Key key,
			Data_Class data);

		//! Deallocates the sentinel node.
		/*!
		Exception safety: nothrow
		Time complexity: constant
		*/
		void deallocateSentinel();

		//! Initializes some member variables.
		/*!
		Exception safety: nothrow
		Time complexity: constant

		This function is to catch functionality
		common to different constructors.
		*/
		void initialize();

		//! Allocates a node.
		/*!
		Exception safety: strong
		Time complexity:	constant
		*/
		Node* allocateNode(
			Key key, 
			Data_Class data,
			Node* parent,
			bool red);

		//! Inserts a new node.
		Node* insert(
			Key&& key, 
			Data_Class&& data, 
			Node* node,
			Node* parent,
			bool fromLeft,
			Node*& newNode);

		//! Removes a node.
		Node* erase(Node* node);

		//! Updates hierarhical data on the path to root.
		/*!
		Calls updateHierarhical(node) for each node in 
		the path to the root, including 'node' itself.
		*/
		void updateToRoot(Node* node);

		void rebalance(Node* toRebalance, bool leftLowOnBlack);

		//! Copy-constructs a subtree.
		/*!
		Exception safety:
		strong, except for not deallocating
		memory in 'allocator', which must be
		done later.

		Time complexity:
		O(n)

		Here 'n' is the number of nodes in the
		subtree of 'thatNode'.
		*/
		Node* copyConstruct(
			Node* parent,
			Node* thatNode);

		//! Destructs the nodes of a subtree.
		/*!
		Exception safety: nothrow
		Time complexity:	O(n)

		Here 'n' is the number of nodes in the subtree
		of 'node'.
		*/
		void clear(Node* node);

		//! Links a parent and a child together.
		/*!
		Preconditions:
		d == Left || d == Right

		Exception safety: nothrow
		Time complexity:	constant
		*/
		void link(Node* parent, Node* child, 
			integer direction);

		//! Tree-rotation
		/*!
		Preconditions:
		d == Left || d == Right

		Exception safety: nothrow
		Time complexity:	constant
		*/
		Node* rotate(Node* node, integer direction);

		//! Flips the colors of a node and its children.
		/*!
		Exception safety: nothrow
		Time complexity:	constant
		*/
		void flipColors(Node* node);

		//! Sets the minimum node.
		/*!
		Exception safety: nothrow
		Time complexity:	constant
		*/
		void setMinimum(Node* node);

		//! Returns the minimum node.
		/*!
		Exception safety: nothrow
		Time complexity:	constant
		*/
		Node* minimum() const;

		//! Sets the maximum node.
		/*!
		Exception safety: nothrow
		Time complexity:	constant
		*/
		void setMaximum(Node* node);

		//! Returns the maximum node.
		/*!
		Exception safety: nothrow
		Time complexity:	constant
		*/
		Node* maximum() const;

		//! The root-node of the tree.
		Node* root_;

		//! The sentinel-node of the tree.
		/*!
		The children of leaf nodes and the parent of the
		root node are set to the sentinel node. 
		This simplifies the implementation by guaranteeing 
		that for any node the children and
		the parent exists. It also works as the
		one-past-last node (end() iterator).
		To make it possible to decrement an iterator
		from end(), the parent of the sentinel node
		is the maximum element, and the left child
		is the sentinel node itself (a unique property
		which can be used to identify it).
		The right child is also the sentinel itself.
		*/
		Node* sentinel_;

		//! The minimum node of the tree.
		Node* minimum_;

		//! The number of stored elements in the tree.
		integer size_;

		//! The comparison functor.
		Compare compare_;
	};

}

// Map

namespace Pastel
{

	template <
		typename Key_, 
		typename Data_,
		typename Compare_ = LessThan,
		integer DereferenceType_ = RedBlackTree_Dereference_Default>
	class Map_Settings
	{
	public:
		using Key = Key_;
		using Data = Data_;
		using Compare = Compare_;
		enum
		{
			DereferenceType = DereferenceType_
		};
	};

	template <
		typename Key, 
		typename Data,
		typename Compare = LessThan,
		integer DereferenceType_ = RedBlackTree_Dereference_Default,
		typename Customization = Empty_RedBlackTree_Customization<
		Map_Settings<Key, Data, Compare, DereferenceType_>>>
	using Map = 
		RedBlackTree<Map_Settings<Key, Data, Compare, DereferenceType_>, Customization>;

}

// Set

namespace Pastel
{

	template <
		typename Key, 
		typename Compare = LessThan,
		integer DereferenceType_ = RedBlackTree_Dereference_Default>
	using Set_Settings = 
		Map_Settings<Key, void, Compare, DereferenceType_>;

	template <
		typename Key, 
		typename Compare = LessThan,
		integer DereferenceType_ = RedBlackTree_Dereference_Default,
		typename Customization = Empty_RedBlackTree_Customization<
		Set_Settings<Key, Compare, DereferenceType_>>>
	using Set = 
		RedBlackTree<Set_Settings<Key, Compare, DereferenceType_>, Customization>;;

}

#include "pastel/sys/redblacktree.hpp"
#include "pastel/sys/redblacktree_private.hpp"
#include "pastel/sys/redblacktree_erase.hpp"
#include "pastel/sys/redblacktree_insert.hpp"
#include "pastel/sys/redblacktree_tools.h"

#endif
