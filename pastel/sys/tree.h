// Description: A generic binary tree

#ifndef PASTEL_TREE_H
#define PASTEL_TREE_H

#include "pastel/sys/destruct.h"
#include "pastel/sys/tree_node.h"
#include "pastel/sys/tree_iterator.h"
#include "pastel/sys/bidirectionaliterator_range.h"

namespace Pastel
{

	//! A generic binary tree
	template <typename Type, typename Derived = EmptyClass>
	class Tree
	{
	public:
		typedef Type Data;

		typedef Tree_Iterator<Data> Iterator;
		typedef Tree_ConstIterator<Data> ConstIterator;

		typedef BidirectionalIterator_Range<Iterator> Range;
		typedef BidirectionalIterator_Range<ConstIterator> ConstRange;

		enum Direction
		{
			Left,
			Right
		};

	private:
		typedef Tree_Private::Node Node;
		typedef Tree_Private::Data_Node<Data> Data_Node;
		typedef Tree_Private::Sentinel_Node Sentinel_Node;

	public:
		//! Construct an empty tree.
		/*!
		Time complexity:
		O(1)

		Exception safety:
		strong
		*/
		Tree()
			: sentinel_(0)
			, leftMost_(0)
			, root_(0)
			, size_(0)
		{
			initialize();
		}

		//! Copy-constructs from another tree.
		/*!
		Time complexity:
		O(that.size())

		Exception safety:
		strong
		*/
		Tree(const Tree& that)
			: sentinel_(0)
			, leftMost_(0)
			, root_(0)
			, size_(0)
		{
			integer rollBack = 0;

			try
			{
				initialize();
				++rollBack;

				if (!that.empty())
				{
					insertRoot(*that.croot());
					++rollBack;

					copyConstruct(that.croot(), croot());
				}
			}
			catch(...)
			{
				switch(rollBack)
				{
				case 2:
					clear();
					// Fall-through.
				case 1:				
					// The destructor will not be run,
					// so remember to deinitialize.
					deinitialize();
					break;
				};

				throw;
			}
		}

		//! Move-constructs from another tree.
		/*!
		Time complexity:
		O(1)

		Exception safety:
		strong
		*/
		Tree(Tree&& that)
			: sentinel_(0)
			, leftMost_(0)
			, root_(0)
			, size_(0)
		{
			// Strong guarantee.
			initialize();
			// Nothrow.
			*this = std::move(that);
		}

		//! Destructs the tree.
		/*!
		Time complexity:
		O(size())

		Exception safety: 
		nothrow
		*/
		~Tree()
		{
			// Nothrow
			clear();
			// Nothrow
			deinitialize();
		}

		//! Copy-assigns from another tree.
		/*!
		Time complexity:
		O(size()) for destruction + 
		O(that.size()) for construction

		Exception safety: 
		strong
		*/
		Tree& operator=(const Tree& that)
		{
			// Note that we want to preserve the sentinel nodes.

			if (this != &that)
			{
				// Detach the current tree.
				Tree temporary(std::move(*this));

				try
				{
					// Create a copy of 'that'.
					insert(end(), Right, that);
				}
				catch(...)
				{
					// Attach the old tree back.
					*this = std::move(temporary);

					throw;
				}
			}

			return *this;
		}

		//! Move-assigns from another tree.
		/*!
		Complexity:
		O(size()) for destruction +	
		O(1) for moving

		Exception safety: 
		nothrow
		*/
		Tree& operator=(Tree&& that)
		{
			// Note that we want to preserve the sentinel nodes.

			if (this != &that)
			{
				// Destruct the current tree.
				clear();

				// Move the stuff from 'that' to this tree.
				root_ = that.root_;
				// Preserve the local sentinel as a parent of root.
				root_->parent = sentinel_;
				leftMost_ = that.leftMost_;
				setRightMost(that.rightMost());
				size_ = that.size_;

				// Forget that 'that' ever owned the stuff.
				that.forget();
			}

			return *this;
		}

		//! Swaps two trees.
		/*!
		Time complexity:
		O(1)

		Exception safety: 
		nothrow
		*/
		void swap(Tree& that)
		{
			using std::swap;

			swap(sentinel_, that.sentinel_);
			swap(leftMost_, that.leftMost_);
			swap(root_, that.root_);
			swap(size_, that.size_);
		}

		//! Returns if the tree is empty.
		/*!
		Time complexity:
		O(1)

		Exception safety: 
		nothrow
		*/
		bool empty() const
		{
			return size_ == 0;
		}

		//! Returns the number of nodes in the tree.
		/*!
		Time complexity:
		O(1)

		Exception safety: 
		nothrow
		*/
		integer size() const
		{
			return size_;
		}
		
		//! Destructs all nodes in the tree.
		/*!
		Time complexity:
		O(size())

		Exception safety: 
		nothrow
		*/
		void clear()
		{
			if (!empty())
			{
				erase((Data_Node*)root_);
			}

			leftMost_ = sentinel_;
			root_ = sentinel_;
			size_ = 0;
		}

		//! Returns the root node.
		/*!
		Time complexity:
		O(1)

		Exception safety: 
		nothrow
		*/
		Iterator root()
		{
			return Iterator(root_);
		}

		//! Returns the root node.
		/*!
		Time complexity:
		O(1)

		Exception safety: 
		nothrow
		*/
		ConstIterator root() const
		{
			return ConstIterator(root_);
		}

		//! Returns the root node.
		/*!
		Time complexity:
		O(1)

		Exception safety: 
		nothrow
		*/
		ConstIterator croot() const
		{
			return ConstIterator(root_);
		}

		//! Returns the first node.
		/*!
		Time complexity:
		O(1)

		Exception safety: 
		nothrow
		*/
		Iterator begin()
		{
			return Iterator(leftMost_);
		}

		//! Returns the first node.
		/*!
		Time complexity:
		O(1)

		Exception safety: 
		nothrow
		*/
		ConstIterator begin() const
		{
			return ConstIterator(leftMost_);
		}

		//! Returns the first node.
		/*!
		Time complexity:
		O(1)

		Exception safety: 
		nothrow
		*/
		ConstIterator cbegin() const
		{
			return ConstIterator(leftMost_);
		}

		//! Returns the one-past-last node.
		/*!
		Time complexity:
		O(1)

		Exception safety: 
		nothrow
		*/
		Iterator end()
		{
			return Iterator(sentinel_);
		}

		//! Returns the one-past-last node.
		/*!
		Time complexity:
		O(1)

		Exception safety: 
		nothrow
		*/
		ConstIterator end() const
		{
			return ConstIterator(sentinel_);
		}

		//! Returns the one-past-last node.
		/*!
		Time complexity:
		O(1)

		Exception safety: 
		nothrow
		*/
		ConstIterator cend() const
		{
			return ConstIterator(sentinel_);
		}

		//! Returns a range to the element sequence.
		/*!
		Time complexity:
		O(1)

		Exception safety: 
		nothrow
		*/
		Range range()
		{
			return Range(begin(), end(), size_);
		}

		//! Returns a range to the element sequence.
		/*!
		Time complexity:
		O(1)

		Exception safety: 
		nothrow
		*/
		ConstRange range() const
		{
			return ConstRange(begin(), end(), size_);
		}

		//! Returns a range to the element sequence.
		/*!
		Time complexity:
		O(1)

		Exception safety: 
		nothrow
		*/
		ConstRange crange() const
		{
			return ConstRange(cbegin(), cend(), size_);
		}

		//! Inserts an element into the root.
		/*!
		Preconditions:
		empty()

		Time complexity:
		O(1)

		Exception safety:
		strong

		returns:
		An iterator to the root node.
		*/
		Iterator insertRoot(const Data& data = Data())
		{
			ENSURE(empty());

			// The children of the sentinel node always
			// refer to itself. Thus we do not set the
			// child pointer here.
			Node* node = allocate(data);

			// Since this is the first actual node in
			// the tree, it is leftmost, rightmost and
			// the root.
			leftMost_ = node;
			setRightMost(node);
			root_ = node;

			return Iterator(node);
		}

		//! Inserts a new node.
		/*!
		Time complexity:
		O(1)

		Exception safety:
		strong

		returns:
		An iterator to the inserted node.
		*/
		Iterator insert(
			const ConstIterator& here, 
			integer childIndex,
			const Data& data = Data())
		{
			ENSURE_OP(childIndex, >=, 0);
			ENSURE_OP(childIndex, <, 2);

			const bool insertAtSentinel =
				here.sentinel();
			ENSURE(!insertAtSentinel);

			Node* parent = (Node*)here.node_;

			Node* node = allocate(data);
			node->parent = parent;

			Node* parentChild = parent->child(childIndex);

			const bool childAlreadyExists = 
				!(parentChild->sentinel());
			ENSURE(!childAlreadyExists);

			parent->setChild(childIndex, node);

			if (childIndex == Left)
			{
				if (parent == leftMost_)
				{
					// If the parent node is the leftmost node, 
					// and we are inserting on the left, then 
					// we have a new leftmost node.
					leftMost_ = node;
				}
			}
			else
			{
				if (parent == rightMost())
				{
					// If the parent node is the rightmost node, 
					// and we are inserting on the right, then 
					// we have a new rightmost node.
					setRightMost(node);
				}
			}

			return Iterator(node);
		}

		//! Copies a tree under a node.
		/*!
		Time complexity:
		O(that.size())

		Exception safety:
		strong

		returns:
		An iterator to the root of the copied tree.
		*/
		Iterator insert(
			const ConstIterator& there, 
			integer childIndex,
			const Tree& that)
		{
			PENSURE_OP(childIndex, >=, 0);
			PENSURE_OP(childIndex, <, 2);

			if (that.empty())
			{
				return cast(there);
			}			

			integer rollback = 0;
			Iterator copyRoot;
			try
			{
				if (there.sentinel())
				{
					copyRoot = insertRoot(*that.root());
				}
				else
				{
					copyRoot = insert(there, childIndex, *that.root());
				}
				++rollback;

				copyConstruct(that.root(), copyRoot, copyRoot);
				++rollback;
			}
			catch(...)
			{
				switch(rollback)
				{
				case 1:
					erase(copyRoot);
					break;
				};
				
				throw;
			}

			return copyRoot;
		}

		//! Moves a tree under a node.
		/*!
		Time complexity:
		O(1)

		Exception safety:
		nothrow

		returns:
		An iterator to the root of the moved tree.
		*/
		Iterator insert(
			const ConstIterator& there, 
			integer childIndex,
			Tree&& that)
		{
			PENSURE_OP(childIndex, >=, 0);
			PENSURE_OP(childIndex, <, 2);

			if (this == &that)
			{
				// Trying to move the tre as a subtree of itself.
				// Copy instead.
				return insert(there, childIndex, (const Tree&)that);
			}

			if (that.empty())
			{
				return end();
			}

			Node* thatRoot = that.root_;

			if (there.sentinel())
			{
				const bool emptyWhenInsertingToSentinel =
					empty();
				ENSURE(emptyWhenInsertingToSentinel);

				*this = std::move(that);
			}
			else
			{			
				Node* thereNode = (Node*)there.node_;

				Node* child = thereNode->child(childIndex);
				
				// Check that the child does not already exist.
				const bool childAlreadyExists = !child->sentinel();
				ENSURE(!childAlreadyExists);

				// Link 'that' tree to 'there' node.
				thereNode->setChild(childIndex, thatRoot);
				thatNode->parent = thereNode;

				// Possibly update the leftmost node.
				if (childIndex == Left)
				{
					if (thereNode == leftMost_)
					{
						leftMost_ = that.leftMost_;
					}
				}

				// Possibly update the rightmost node.
				if (childIndex == Right)		
				{				
					if (thereNode == rightMost())
					{
						setRightMost(that.rightMost());
					}
				}

				// Update the size.
				size_ += that.size_;

				// Forget that 'that' ever owned the stuff.
				that.forget();
			}

			return Iterator(thatRoot);
		}

		//! Removes a node.
		/*!
		Time complexity:
		O(size(node))

		Exception safety:
		nothrow
		*/
		void erase(const ConstIterator& that)
		{
			PENSURE(!that.sentinel());

			erase((Data_Node*)that.node_);
		}

		//! Detaches a sub-tree to its own tree.
		/*!
		Time complexity:
		O(size(node))

		Exception safety:
		strong
		*/
		Tree detach(const ConstIterator& that)
		{
			PENSURE(!that.sentinel());

			Node* detachedRoot = (Node*)that.node_;
			Node* oldParent = detachedRoot->parent;

			// Compute leftmost node, rightmost node,
			// and the size of the sub-tree.
			Node* detachedLeftMost = leftMost_; 
			Node* detachedRightMost = rightMost();
			integer detachedSize = size_;
			if (detachedRoot != root_)
			{
				// We only need to compute these if
				// the node is not the root node. This
				// is the common case, but we optimize
				// the root case anyway.
				detachedLeftMost = extremum(detachedRoot, Left);
				detachedRightMost = extremum(detachedRoot, Right);
				detachedSize = size(detachedRoot);
			}

			// Detach the sub-tree to its own tree.
			Tree detached;
			detachedRoot->parent = detached.sentinel_;
			detached.root_ = detachedRoot;
			detached.leftMost_ = detachedLeftMost;
			detached.setRightMost(detachedRightMost);
			detached.size_ = detachedSize;

			// Update the removed child of the parent.
			for (integer i = 0;i < 2;++i)
			{
				if (oldParent->child(i) == detachedRoot)
				{
					oldParent->setChild(i, sentinel_);
					break;
				}
			}

			// Update the root node if necessary.
			if (detachedRoot == root_)
			{
				root_ = sentinel_;
			}

			// Update the leftmost node if necessary.
			if (detachedLeftMost == leftMost_)
			{
				leftMost_ = oldParent;
			}

			// Update the rightmost node if necessary.
			if (detachedRightMost == rightMost())
			{
				setRightMost(oldParent);
			}

			// Update the size.
			size_ -= detachedSize;

			return detached;
		}

		//! Rotates a node.
		/*!
		Preconditions:
		!that.child(R).child(L).sentinel()
		where L = direction, and R = !L.

		Time complexity:
		O(1)

		Exception safety:
		nothrow
		*/
		void rotate(
			const ConstIterator& that,
			integer direction)
		{
			PENSURE_OP(direction, >=, 0);
			PENSURE_OP(direction, <, 2);

			const integer L = direction;
			const integer R = !L;

			ConstIterator right = that.child(R);
			ConstIterator rightLeft = right.child(L);

			const bool rotationWellDefined = 
				!rightLeft.sentinel();
			ENSURE(rotationWellDefined);

			Node* thatNode = (Node*)that.node_;
			Node* rightNode = (Node*)right.node_;
			Node* rightLeftNode = (Node*)rightLeft.node_;
			Node* thatParent = thatNode->parent;

			thatNode->setChild(R, rightLeftNode);
			rightLeftNode->parent = thatNode;

			rightNode->setChild(L, thatNode);
			thatNode->parent = rightNode;

			rightNode->parent = thatParent;

			if (thatNode == root_)
			{
				root_ = rightNode;
			}
		}

		//! Converts a const-iterator to an iterator.
		/*!
		Time complexity:
		O(1)

		Exception safety:
		nothrow
		*/
		Iterator cast(const ConstIterator& that)
		{
			return Iterator((Node*)that.node_);
		}

		//! Returns the reference count of the sentinel node.
		/*!
		Time complexity:
		O(1)

		Exception safety:
		nothrow

		This function is mainly useful for testing purposes.
		Each tree has a local sentinel node which works as
		the one-past-end node for iterators. However, the
		sentinel nodes are also used as child references to
		denote an empty child. Here the sentinel nodes may
		be shared between trees, and their lifetime is
		tracked using reference counting.
		*/
		integer sentinelCount() const
		{
			return sentinel_->count();
		}

	private:
		//! Initializations common to all constructors.
		/*!
		Time complexity:
		O(1)

		Exception safety:
		strong
		*/
		void initialize()
		{
			sentinel_ = new Sentinel_Node;
			sentinel_->increaseCount();

			leftMost_ = sentinel_;
			root_ = sentinel_;
		}

		//! Deinitializations common to all 'destructors'.
		/*!
		Time complexity:
		O(1)

		Exception safety:
		nothrow
		*/
		void deinitialize()
		{
			// Note that this line may delete
			// the sentinel node.
			sentinel_->decreaseCount();
			sentinel_ = 0;
			leftMost_ = 0;
			size_ = 0;
		}

		//! Traverses left (right) as much as possible.
		/*!
		Time complexity:
		O(size(node))

		Exception safety:
		nothrow
		*/
		Node* extremum(
			Node* node,
			Direction direction)
		{
			ASSERT(!node->sentinel());

			Node* next = node;
			do
			{
				node = next;
				next = node->child(direction);
			}
			while(!next->sentinel());

			return node;
		}

		//! Computes the size of a subtree recursively.
		/*!
		Time complexity:
		O(size(node))

		Exception safety:
		nothrow
		*/
		integer size(Node* node) const
		{
			ASSERT(node);

			integer result = 0;

			for (integer i = 0;i < 2;++i)
			{
				Node* child = node->child(i);
				if (!child->sentinel())
				{
					result += size(child);
				}
			}

			if (!node->sentinel())
			{
				result += 1;
			}

			return result;
		}

		//! Removes a subtree recursively.
		/*!
		Time complexity:
		O(size(node))

		Exception safety:
		nothrow
		*/
		void erase(Data_Node* node)
		{
			ASSERT(node);
			ASSERT(!node->sentinel());

			Node* parent = node->parent;

			eraseSubtree(node);

			for (integer i = 0;i < 2;++i)
			{
				if (parent->child(i) == node)
				{
					parent->setChild(i, sentinel_);
					break;
				}
			}
		}

		//! Removes a subtree recursively.
		/*!
		Time complexity:
		O(size(node))

		Exception safety:
		nothrow
		*/
		void eraseSubtree(Data_Node* node)
		{
			ASSERT(node);
			ASSERT(!node->sentinel());

			// Recurse to remove child subtrees.
			for (integer i = 0;i < 2;++i)
			{
				Node* child = node->child(i);
				if (!child->sentinel())
				{
					eraseSubtree((Data_Node*)child);
				}
			}

			Node* parent = node->parent;
			if (node == leftMost_)
			{
				// We are removing the leftmost node.
				// Make the parent node the leftmost node.
				leftMost_ = parent;
			}

			if (node == rightMost())
			{
				// We are removing the rightmost node.
				// Make the parent node the rightmost node.
				setRightMost(parent);
			}

			if (node == root_)
			{
				// We are removing the root node.
				// Make sentinel node the root node.
				root_ = sentinel_;
			}
			
			deallocate(node);
		}

		//! Allocates a data node.
		/*!
		Time complexity:
		O(1)

		Exception safety:
		strong
		*/
		Data_Node* allocate(const Data& data)
		{
			integer rollBack = 0;
			Data_Node* node = 0;

			try
			{
				node = new Data_Node(sentinel_);
				++rollBack;
			
				if (node->data())
				{
					new (node->data()) Data(data);
					++rollBack;
				}
			}
			catch(...)
			{
				switch(rollBack)
				{
				case 1:
					delete node;
					break;
				};

				throw;
			}

			++size_;

			return node;
		}

		//! Deallocates a data node.
		/*!
		Time complexity:
		O(1)

		Exception safety:
		nothrow
		*/
		void deallocate(Data_Node* node)
		{
			ASSERT(node);
			ASSERT(!node->sentinel());

			if (node->data())
			{
				destruct(node->data());
			}
			delete node;

			--size_;

			ASSERT_OP(size_, >=, 0);
		}

		void copyConstruct(
			const ConstIterator& from, 
			const ConstIterator& to,
			const ConstIterator& forbidden = ConstIterator())
		{
			ASSERT(!from.sentinel());
			ASSERT(!to.sentinel());

			for (integer i = 0;i < 2;++i)
			{
				const ConstIterator newFrom =
					from.child(i);
				if (!newFrom.sentinel() &&
					newFrom != forbidden)
				{
					const ConstIterator newTo = 
						insert(to, i, *newFrom);

					copyConstruct(newFrom, newTo, forbidden);
				}
			}
		}

		//! Sets the rightmost node.
		/*!
		Time complexity:
		O(1)

		Exception safety:
		nothrow

		The rightmost node is stored at the parent of
		the sentinel node, since this way the iterator
		naturally traverses between one-past-last (end())
		and the rightmost node.
		*/
		void setRightMost(Node* node)
		{
			sentinel_->parent = node;
			if (!node->sentinel())
			{
				node->setChild(Right, sentinel_);
			}
		}

		//! Returns the rightmost node.
		/*!
		Time complexity:
		O(1)

		Exception safety:
		nothrow
		*/
		Node* rightMost() const
		{
			return sentinel_->parent;
		}

		//! Sets all to initial position without destructions.
		/*!
		Time complexity:
		O(1)

		Exception safety:
		nothrow
		*/
		void forget()
		{
			root_ = sentinel_;
			leftMost_ = sentinel_;
			setRightMost(sentinel_);
			size_ = 0;
		}

		Sentinel_Node* sentinel_;
		Node* leftMost_;
		Node* root_;
		integer size_;
	};

}

#endif
