// Description: A red-black tree

#ifndef PASTEL_REDBLACKTREE_H
#define PASTEL_REDBLACKTREE_H

#include "pastel/sys/mytypes.h"
#include "pastel/sys/redblacktree_concepts.h"
#include "pastel/sys/redblacktree_node.h"
#include "pastel/sys/redblacktree_iterator.h"
#include "pastel/sys/object_forwarding.h"

namespace Pastel
{

	//! A red-black tree
	template <
		typename Key,
		typename Compare = LessThan,
		typename Data = void,
		typename Customization = RedBlackTree_Concepts::Customization<Key, Compare, Data>>
	class RedBlackTree
		: public Customization
	{
	public:
		typedef RedBlackTree_Fwd<Key, Compare, Data> Fwd;
		typedef Customization Customization_;

		PASTEL_FWD(Iterator);
		PASTEL_FWD(ConstIterator);
		PASTEL_FWD(Data_Class);

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
		template <typename Key_ConstIterator>
		void insertMany(
			Key_ConstIterator begin,
			Key_ConstIterator end);

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

		//! Searches for a node with the given key.
		/*!
		Exception safety: nothrow
		Time complexity: O(log n)
		*/
		ConstIterator find(const Key& key) const;

		//! Returns the iterator to the smallest element.
		/*!
		See the documentation for the const version.
		*/
		Iterator begin();

		//! Returns the iterator to the smallest element.
		/*!
		Exception safety: nothrow
		Time complexity: constant
		*/
		ConstIterator cbegin() const;

		//! Returns the iterator to the one-past-greatest element.
		/*!
		See the documentation for the const version.
		*/
		Iterator end();

		//! Returns the iterator to the one-past-greatest element.
		/*!
		Exception safety: nothrow
		Time complexity: constant
		*/
		ConstIterator cend() const;

		//! Returns the iterator to the greatest element.
		/*!
		See the documentation for the const version.
		*/
		Iterator last();

		//! Returns the iterator to the greatest element.
		/*!
		Exception safety: nothrow
		Time complexity: constant
		*/
		ConstIterator clast() const;

		//! Returns the iterator to the root element.
		/*!
		See the documentation for the const version.
		*/
		Iterator root();

		//! Returns the iterator to the root element.
		/*!
		Exception safety: nothrow
		Time complexity: constant
		*/
		ConstIterator croot() const;

	private:
		enum
		{
			Left = 0,
			Right = 1
		};

		typedef RedBlackTree_::Node<Key, Data> Node;

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
			Key key, 
			Data_Class data, 
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

#include "pastel/sys/redblacktree.hpp"
#include "pastel/sys/redblacktree_private.hpp"
#include "pastel/sys/redblacktree_erase.hpp"
#include "pastel/sys/redblacktree_insert.hpp"

#endif
