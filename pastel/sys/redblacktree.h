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
		Time complexity: O(1)
		Exception safety: strong
		*/
		explicit RedBlackTree(
			Key sentinelKey = Key(), 
			Data_Class sentinelData = Data_Class());

		//! Copy-constructs from another tree.
		/*!
		Time complexity: O(that.size()) * insert()
		Exception safety: strong
		*/
		RedBlackTree(const RedBlackTree& that);

		//! Move-constructs from another tree.
		/*!
		Time complexity: O(1)
		Exception safety: nothrow
		*/
		RedBlackTree(RedBlackTree&& that);

		//! Replaces this tree with a copy of another tree.
		/*!
		Time complexity: move/copy-construction
		Exception safety: strong
		*/
		RedBlackTree& operator=(RedBlackTree that);

		//! Destructs the tree.
		/*!
		Time complexity: clear()
		Exception safety: nothrow
		*/
		~RedBlackTree();

		//! Swaps two trees.
		/*!
		Time complexity: O(1)
		Exception safety: nothrow
		*/
		void swap(RedBlackTree& that);

		//! Removes all elements from the tree.
		/*!
		Time complexity: O(size()) + onClear()
		Exception safety: nothrow
		*/
		void clear();

		//! Returns the number of elements in the tree.
		/*!
		Time complexity: O(1)
		Exception safety: nothrow
		*/
		integer size() const;

		//! Returns true if the tree is empty.
		/*!
		Time complexity: O(1)
		Exception safety: nothrow
		*/
		bool empty() const;

		//! Inserts an element into the tree.
		/*!
		Time complexity: O(log n) * updateHieararchicalData() + onInsert()
		Exception safety: strong + onInsert()

		Here f(n) is the time spent computing the
		user-specified hierarchical data (usually O(1)).
		*/
		Iterator insert(
			Key key, 
			Data_Class data = Data_Class());

		//! Inserts elements into the tree.
		/*!
		Time complexity: insert() * std::distance(begin, end)
		Exception safety: basic
		*/
		template <typename Key_ConstIterator_>
		void insertMany(
			Key_ConstIterator_ begin,
			Key_ConstIterator_ end);

		//! Removes an element from the tree by its iterator.
		/*!
		Time complexity: O(log n) * updateHierarchicalData() + onErase()
		Exception safety: nothrow
		*/
		Iterator erase(const ConstIterator& that);

		//! Removes an element from the tree by its key.
		/*!
		Time complexity: erase(find(key))
		Exception safety: nothrow
		*/
		Iterator erase(const Key& key);

		//! Splices an element from 'that' tree to this tree.
		/*!
		Time complexity: O(log(size()))
		Exception safety: nothrow

		No constructors or destructors are performed in this
		process; the node containing the element is moved
		from 'that' tree to this tree.
		*/
		Iterator splice(
			RedBlackTree& that, 
			const ConstIterator& thatFrom);

		//! Searches for the first element with key == 'key'.
		/*!
		Time complexity: O(log(size()))
		Exception safety: nothrow
		*/
		Iterator find(const Key& key);
		ConstIterator find(const Key& key) const;

		//! Searches for the first element with key >= 'key'.
		/*!
		Time complexity: O(log(size()))
		Exception safety: nothrow
		*/
		Iterator lowerBound(const Key& key);
		ConstIterator lowerBound(const Key& key) const;

		Iterator lower_bound(const Key& key)
		{
			return lowerBound(key);
		}

		ConstIterator lower_bound(const Key& key) const
		{
			return lowerBound(key);
		}

		//! Searches for the first element with key > 'key'.
		/*!
		Time complexity: O(log(size()))
		Exception safety: nothrow
		*/
		Iterator upperBound(const Key& key);
		ConstIterator upperBound(const Key& key) const;

		Iterator upper_bound(const Key& key)
		{
			return upperBound(key);
		}
		
		ConstIterator upper_bound(const Key& key) const
		{
			return upperBound(key);
		}

		//! Casts away iterator constness.
		/*!
		Time complexity: O(1)
		Exception safety: nothrow
		*/
		Iterator cast(const ConstIterator& that);

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
		Time complexity: constant
		*/
		Node* allocateNode(
			Key&& key, 
			Data_Class&& data,
			bool red);

		//! Inserts a new node.
		Node* insert(
			Node* newNode,
			Node* node,
			Node* parent,
			bool fromLeft);

		//! Removes a node.
		std::pair<Node*, Node*> erase(Node* node);

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

		//! Find either the lower or the upper bound of a key.
		/*!
		The lower bound is the first element with key >= 'key';
		it is given by Direction = Right.
		The upper bound is the first element with key > 'key';
		it is given by Direction = Left.
		*/
		template <bool Direction>
		ConstIterator bound(const Key& key) const;

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
	class RedBlack_Map_Settings
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
		RedBlack_Map_Settings<Key, Data, Compare, DereferenceType_ >> >
	using RedBlack_Map = 
		RedBlackTree<RedBlack_Map_Settings<Key, Data, Compare, DereferenceType_>, Customization>;

}

// Set

namespace Pastel
{

	template <
		typename Key, 
		typename Compare = LessThan,
		integer DereferenceType_ = RedBlackTree_Dereference_Default>
	using RedBlack_Set_Settings = 
		RedBlack_Map_Settings<Key, void, Compare, DereferenceType_>;

	template <
		typename Key, 
		typename Compare = LessThan,
		integer DereferenceType_ = RedBlackTree_Dereference_Default,
		typename Customization = Empty_RedBlackTree_Customization<
		RedBlack_Set_Settings<Key, Compare, DereferenceType_>>>
	using RedBlack_Set = 
		RedBlackTree<RedBlack_Set_Settings<Key, Compare, DereferenceType_>, Customization>;;

}

#include "pastel/sys/redblacktree.hpp"
#include "pastel/sys/redblacktree_private.hpp"
#include "pastel/sys/redblacktree_erase.hpp"
#include "pastel/sys/redblacktree_insert.hpp"
#include "pastel/sys/redblacktree_tools.h"

#endif
