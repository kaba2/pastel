#ifndef PASTEL_REDBLACKTREE_H
#define PASTEL_REDBLACKTREE_H

#include "pastel/sys/mytypes.h"
#include "pastel/sys/poolallocator.h"

#include <boost/type_traits/is_same.hpp>

#include "pastel/sys/redblacktree_node.h"
#include "pastel/sys/redblacktree_iterator.h"

namespace Pastel
{

	class RbtPolicy_Concept
	{
	public:
	};

	class No_RbtPolicy
	{
	public:
		typedef EmptyClass ValueType;
	};

	template <typename Value>
	class Value_RbtPolicy
	{
	public:
		typedef Value ValueType;
	};

	//! Left-leaning red-black tree
	template <
		typename Key,
		typename Compare = std::less<Key>, 
		typename RbtPolicy = No_RbtPolicy>
	class RedBlackTree
	{
	public:
		typedef Key KeyType;
		typedef typename RbtPolicy::ValueType ValueType;
		typedef Compare CompareType;

		typedef RedBlackTree_Detail::Iterator<Key, ValueType> Iterator;
		typedef RedBlackTree_Detail::ConstIterator<Key, ValueType> ConstIterator;

		//! Constructs an empty tree.
		RedBlackTree();

		//! Constructs a copy of another tree.
		RedBlackTree(const RedBlackTree& that);

		//! Replaces this tree with a copy of another tree.
		RedBlackTree& operator=(const RedBlackTree& that);

		//! Destructs the tree.
		~RedBlackTree();

		//! Swaps two trees.
		void swap(RedBlackTree& that);

		//! Removes all elements from the tree.
		void clear();

		//! Returns true if the tree is empty.
		bool empty() const;

		//! Inserts an element into the tree.
		Iterator insert(
			const Key& key, 
			const ValueType& value = ValueType());

		//! Removes an element from the tree.
		Iterator erase(const ConstIterator& that);

		//! Searches for a node with the given value.
		Iterator find(const Key& key);

		//! Searches for a node with the given value.
		ConstIterator find(const Key& key) const;

		//! Returns the iterator to the smallest element.
		Iterator begin();

		//! Returns the iterator to the smallest element.
		ConstIterator begin() const;

		//! Returns the iterator to the one-past-greatest element.
		Iterator end();

		//! Returns the iterator to the one-past-greatest element.
		ConstIterator end() const;

		//! Returns the iterator to the greatest element.
		Iterator last();

		//! Returns the iterator to the greatest element.
		ConstIterator last() const;

	private:
		enum
		{
			ValueExists = 
			!boost::is_same<ValueType, EmptyClass>::value
		};

		typedef RedBlackTree_Detail::Color Color;
		typedef RedBlackTree_Detail::Node<Key, ValueType> Node;

		//! Allocates the sentinel node etc.
		/*!
		Exception safety: 
		strong
		*/
		void initialize();

		//! Allocates a node.
		/*!
		Exception safety:
		strong
		*/
		Node* allocateNode(
			const Key& key, 
			const ValueType* value,
			Node* parent);

		//! Destruct a node.
		void destructNode(Node* node);
		
		//! Inserts a new node.
		Node* insert(
			const Key& key, 
			const ValueType* value, 
			Node* node,
			Node* parent,
			bool fromLeft,
			Node*& newNode);

		//! Copy-constructs a subtree.
		/*!
		Exception safety:
		strong, except for not deallocating
		memory in 'allocator', which must be
		done later.
		*/
		Node* constructNode(
			Node* parent,
			Node* thatNode);

		//! Destructs the nodes of a subtree.
		void clear(Node* node);

		//! Tree-rotation to the left.
		Node* rotateLeft(Node* node);

		//! Tree-rotation to the right.
		Node* rotateRight(Node* node);

		Node* moveRedLeft(Node* node);
		Node* moveRedRight(Node* node);

		//! Flips the colors of a node and its children.
		void flipColors(Node* node);

		//! Sets the minimum node.
		void setMinimum(Node* node);

		//! Returns the minimum node.
		Node* minimum() const;

		//! Sets the maximum node.
		void setMaximum(Node* node);

		//! Returns the maximum node.
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

		//! The allocator for the nodes.
		PoolAllocator allocator_;

		//! The comparison functor.
		Compare compare_;
	};

}

#include "pastel/sys/redblacktree.hpp"
#include "pastel/sys/redblacktree_private.hpp"

#endif
