// Description: RedBlackTree class

#ifndef PASTEL_REDBLACKTREE_H
#define PASTEL_REDBLACKTREE_H

#include "pastel/sys/mytypes.h"
#include "pastel/sys/poolallocator.h"
#include "pastel/sys/rbtpolicy_concept.h"
#include "pastel/sys/set_rbtpolicy.h"
#include "pastel/sys/redblacktree_node.h"
#include "pastel/sys/redblacktree_iterator.h"

namespace Pastel
{

	//! Left-leaning red-black tree
	template <
		typename Key,
		typename Compare = std::less<Key>, 
		typename RbtPolicy = Set_RbtPolicy>
	class RedBlackTree
	{
	public:
		typedef Key KeyType;
		typedef typename RbtPolicy::ValueType ValueType;
		typedef Compare CompareType;

		typedef RedBlackTree_Detail::Iterator<Key, ValueType> Iterator;
		typedef RedBlackTree_Detail::ConstIterator<Key, ValueType> ConstIterator;

		//! Constructs an empty tree.
		/*!
		Exception safety: strong
		Complexity: constant
		*/
		explicit RedBlackTree(
			const RbtPolicy& policy = RbtPolicy(),
			const Key& sentinelKey = Key(), 
			const ValueType& sentinelValue = ValueType());

		//! Constructs a copy of another tree.
		/*!
		Exception safety: strong
		Complexity: O(n)
		*/
		RedBlackTree(
			const RedBlackTree& that,
			const RbtPolicy& policy = RbtPolicy());

		//! Replaces this tree with a copy of another tree.
		/*!
		Exception safety: strong
		Complexity: O(n)
		*/
		RedBlackTree& operator=(const RedBlackTree& that);

		//! Destructs the tree.
		/*!
		Exception safety: nothrow
		Complexity: O(n)
		*/
		~RedBlackTree();

		//! Swaps two trees.
		/*!
		Exception safety: nothrow
		Complexity: constant
		*/
		void swap(RedBlackTree& that);

		//! Removes all elements from the tree.
		/*!
		Exception safety: nothrow
		Complexity: O(n)
		*/
		void clear();

		//! Returns the number of elements in the tree.
		/*!
		Exception safety: nothrow
		Complexity: constant
		*/
		integer size() const;

		//! Returns true if the tree is empty.
		/*!
		Exception safety: nothrow
		Complexity: constant
		*/
		bool empty() const;

		//! Inserts an element into the tree.
		/*!
		Exception safety: nothrow
		Complexity: O(f(n) log n)

		Here f(n) is the time spent computing the
		user-specified hierarchical data (usually O(1)).
		*/
		Iterator insert(
			const Key& key, 
			const ValueType& value = ValueType());

		//! Removes an element from the tree.
		/*!
		Exception safety: nothrow
		Complexity: O(f(n) log n)

		Here f(n) is the time spent computing the
		user-specified hierarchical data (usually O(1)).
		*/
		Iterator erase(const ConstIterator& that);

		//! Removes an element from the tree.
		/*!
		This is a convenience function that calls
		erase(find(key)).
		*/
		Iterator erase(const Key& key);

		//! Searches for a node with the given value.
		/*!
		See the documentation for the const version.
		*/
		Iterator find(const Key& key);

		//! Searches for a node with the given value.
		/*!
		Exception safety: nothrow
		Complexity: O(log n)
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
		Complexity: constant
		*/
		ConstIterator begin() const;

		//! Returns the iterator to the one-past-greatest element.
		/*!
		See the documentation for the const version.
		*/
		Iterator end();

		//! Returns the iterator to the one-past-greatest element.
		/*!
		Exception safety: nothrow
		Complexity: constant
		*/
		ConstIterator end() const;

		//! Returns the iterator to the greatest element.
		/*!
		See the documentation for the const version.
		*/
		Iterator last();

		//! Returns the iterator to the greatest element.
		/*!
		Exception safety: nothrow
		Complexity: constant
		*/
		ConstIterator last() const;

		//! Returns the iterator to the root element.
		/*!
		See the documentation for the const version.
		*/
		Iterator root();

		//! Returns the iterator to the root element.
		/*!
		Exception safety: nothrow
		Complexity: constant
		*/
		ConstIterator root() const;

	private:
		enum
		{
			Left = 0,
			Right = 1
		};

		typedef RedBlackTree_Detail::Node<Key, ValueType> Node;

		//! Allocates the sentinel node.
		/*!
		Exception safety: strong
		Complexity: constant
		*/
		void allocateSentinel(
			const Key& sentinelKey,
			const ValueType* sentinelValue);

		//! Deallocates the sentinel node.
		/*!
		Exception safety: nothrow
		Complexity: constant
		*/
		void deallocateSentinel();

		//! Initializes some member variables.
		/*!
		Exception safety: nothrow
		Complexity: constant

		This function is to catch functionality
		common to different constructors.
		*/
		void initialize();

		//! Allocates a node.
		/*!
		Exception safety: strong
		Complexity:	constant
		*/
		Node* allocateNode(
			const Key& key, 
			const ValueType* value,
			Node* parent,
			bool red);

		//! Destruct a node.
		/*!
		Exception safety: nothrow
		Complexity: constant
		*/
		void destructNode(Node* node);
		
		//! Inserts a new node.
		Node* insert(
			const Key& key, 
			const ValueType* value, 
			Node* node,
			Node* parent,
			bool fromLeft,
			Node*& newNode);

		//! Removes a node.
		Node* erase(Node* node);

		//! Updates hierarhical data on the path to root.
		/*!
		Calls policy_.updateHierarhical(node) for each node in 
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

		Complexity:
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
		Complexity:	O(n)

		Here 'n' is the number of nodes in the subtree
		of 'node'.
		*/
		void clear(Node* node);

		//! Links a parent and a child together.
		/*!
		Preconditions:
		d == Left || d == Right

		Exception safety: nothrow
		Complexity:	constant
		*/
		void link(Node* parent, Node* child, 
			integer direction);

		//! Tree-rotation
		/*!
		Preconditions:
		d == Left || d == Right

		Exception safety: nothrow
		Complexity:	constant
		*/
		Node* rotate(Node* node, integer direction);

		//! Flips the colors of a node and its children.
		/*!
		Exception safety: nothrow
		Complexity:	constant
		*/
		void flipColors(Node* node);

		//! Sets the minimum node.
		/*!
		Exception safety: nothrow
		Complexity:	constant
		*/
		void setMinimum(Node* node);

		//! Returns the minimum node.
		/*!
		Exception safety: nothrow
		Complexity:	constant
		*/
		Node* minimum() const;

		//! Sets the maximum node.
		/*!
		Exception safety: nothrow
		Complexity:	constant
		*/
		void setMaximum(Node* node);

		//! Returns the maximum node.
		/*!
		Exception safety: nothrow
		Complexity:	constant
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

		//! The allocator for the nodes.
		PoolAllocator allocator_;

		//! The comparison functor.
		Compare compare_;

		//! The policy.
		RbtPolicy policy_;
	};

}

#include "pastel/sys/redblacktree.hpp"
#include "pastel/sys/redblacktree_private.hpp"
#include "pastel/sys/redblacktree_erase.hpp"
#include "pastel/sys/redblacktree_insert.hpp"

#endif
