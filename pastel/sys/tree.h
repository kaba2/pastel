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

		enum
		{
			Left,
			Right
		};

	private:
		typedef Tree_Private::Node<Data> Node;

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
			initialize();

			try
			{
				if (!that.empty())
				{
					insertRoot(*that.croot());

					copyConstruct(that.croot(), croot());
				}
			}
			catch(...)
			{
				clear();
				
				// The destructor will not be run,
				// so remember to deinitialize.
				deinitialize();

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
			initialize();

			// The sentinel node of 'that' is not
			// preserved. This is ok, since
			// 'that' will not be used anymore.
			swap(that);
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
			clear();
			deinitialize();
		}

		//! Copy-assigns from another tree.
		/*!
		Time complexity:
		O(size() + that.size())

		Exception safety: 
		strong
		*/
		Tree& operator=(const Tree& that)
		{
			// Note that we want to preserve the sentinel node.
			// This is why we do this rather than copy and swap.

			// Detach the current tree.
			Node* oldRoot = root_;
			Node* oldLeftMost = leftMost_;
			Node* oldRightMost = sentinel_->parent;
			integer oldSize = size_;

			root_ = sentinel_;
			leftMost_ = sentinel_;
			sentinel_->parent = sentinel_;
			size_ = 0;

			try
			{
				// Create a copy of 'that'.
				Iterator copyRoot = 
					insert(end(), Tree::Right, that);
			}
			catch(...)
			{
				// Attach the old tree back.
				root_ = oldRoot;
				leftMost_ = oldLeftMost;
				sentinel_->parent = oldRightMost;
				size_ = oldSize;

				throw;
			}

			Node* newRoot = root_;
			Node* newLeftMost = leftMost_;
			Node* newRightMost = sentinel_->parent;
			integer newSize = size_;

			// Switch the old tree back.
			root_ = oldRoot;
			leftMost_ = oldLeftMost;
			sentinel_->parent = oldRightMost;
			size_ = oldSize;

			// Destruct the old tree.
			clear();

			// Attach the copy of 'that' back.
			root_ = newRoot;
			leftMost_ = newLeftMost;
			sentinel_->parent = newRightMost;
			size_ = newSize;
			
			return *this;
		}

		//! Move-assigns from another tree.
		/*!
		Complexity:
		O(size() + that.size())

		Exception safety: 
		strong
		*/
		Tree& operator=(Tree&& that)
		{
			// Note that we want to preserve the sentinel nodes.

			clear();

			root_ = that.root_;
			leftMost_ = that.leftMost_;
			sentinel_->parent = that.sentinel_->parent;
			size_ = that.size_;

			that.root_ = that.sentinel_;
			that.leftMost_ = that.sentinel_;
			that.sentinel_->parent = that.sentinel_;
			that.size_ = 0;

			updateSentinel(root_);
			
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
				erase(root_);
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

			// The rightmost node is stored at the
			// sentinel parent link. 
			Node*& rightMost = sentinel_->parent;

			// Since this is the first actual node in
			// the tree, it is leftmost, rightmost and
			// the root.
			leftMost_ = node;
			rightMost = node;
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

			Node*& parentChild = parent->childSet[childIndex];

			const bool childAlreadyExists = 
				!(parentChild->sentinel());
			ENSURE(!childAlreadyExists);

			const bool iteratorFromAnotherTree = 
				(parentChild != sentinel_);
			ENSURE(!iteratorFromAnotherTree);

			// The rightmost node is stored at the
			// sentinel parent link. 
			Node*& rightMost = sentinel_->parent;

			parentChild = node;

			if (childIndex == 0)
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
				if (parent == rightMost)
				{
					// If the parent node is the rightmost node, 
					// and we are inserting on the right, then 
					// we have a new rightmost node.
					rightMost = node;
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
		O(that.size())

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

			if (that.empty())
			{
				return;
			}

			Node* thatRoot = that.root_;

			if (there.sentinel())
			{
				const bool emptyWhenInsertingToSentinel =
					empty();
				ENSURE(emptyWhenInsertingToSentinel);

				root_ = thatRoot;
				leftMost_ = that.leftMost_;
				rightMost_ = that.rightMost_;
				size_ = that.size_;
			}
			else
			{			
				Node* thereNode = (Node*)there.node_;

				Node*& childRef = thereNode->childSet[childIndex];
				
				// Check that the child does not already exist.
				const bool childAlreadyExists = 
					(childRef != sentinel_);
				ENSURE(!childAlreadyExists);

				// Link 'that' tree to 'there' node.
				childRef = thatRoot;
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
					Node*& rightMost = sentinel_->parent;
					if (thereNode == rightMost)
					{
						rightMost = that.rightMost_;
					}
				}

				// Update the size.
				size_ += that.size_;
			}

			// Change the sentinel nodes to refer to 
			// this tree.
			updateSentinel(thatRoot);

			// Get rid of the tree in 'that'.
			that.leftMost_ = that.sentinel_;
			that.root_ = that.sentinel_;
			that.sentinel_->parent_ = that.sentinel_;
			that.size_ = 0;

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

			erase((Node*)that.node_);
		}

		Tree detach(const ConstIterator& that)
		{
			ENSURE(!that.sentinel());

			Node* node = (Node*)that.node_;
			const integer detachedSize = size(node);

			// Detach the subtree given by 'that' 
			// from this tree.

			Node* parent = node->parent;
			integer index = 0;
			while (index < 2 && parent->childSet[index] != node)
			{
				++index;
			}

			parent->childSet[index] = sentinel_;

			size_ -= detachedSize;

			// Attach the subtree to a new tree.
			
			Tree tree;
			node->parent = tree.sentinel_;
			tree.sentinel_->childSet[0] = node;
			tree.size_ = detachedSize;

			return tree;
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

			thatNode->childSet[R] = rightLeftNode;
			rightLeftNode->parent = thatNode;

			rightNode->childSet[L] = thatNode;
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
			// The sentinel node should not be
			// constructed with allocate(). It
			// has no user data.
			sentinel_ = new Node;

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
			// The sentinel node should not be
			// destructed with deallocate(). It
			// has no user data.
			delete sentinel_;
			sentinel_ = 0;
			leftMost_ = 0;
			size_ = 0;
		}

		//! Computes the size of a subtree recursively.
		/*!
		Time complexity:
		O(size(node))

		Exception safety:
		nothrow
		*/
		integer size(Node* node)
		{
			ASSERT(node);

			integer result = 0;

			for (integer i = 0;i < 2;++i)
			{
				Node* child = node->child(i);
				if (child != sentinel_)
				{
					result += size(child);
				}
			}

			if (node != sentinel_)
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
		void erase(Node* node)
		{
			ASSERT(node);
			ASSERT(node != sentinel_);

			for (integer i = 0;i < 2;++i)
			{
				Node* child = node->childSet[i];
				if (child != sentinel_)
				{
					erase(child);
				}
			}

			Node* parent = node->parent;
			for (integer i = 0;i < 2;++i)
			{
				if (parent->childSet[i] == node)
				{
					parent->childSet[i] = sentinel_;
					break;
				}
			}

			if (node == leftMost_)
			{
				leftMost_ = parent;
			}

			Node*& rightMost = sentinel_->parent;
			if (node == rightMost)
			{
				rightMost = parent;
			}

			if (node == root_)
			{
				root_ = sentinel_;
			}
			
			deallocate(node);
		}

		//! Allocates a node.
		/*!
		Time complexity:
		O(1)

		Exception safety:
		strong
		*/
		Node* allocate(const Data& data)
		{
			integer rollBack = 0;
			Node* node = 0;

			try
			{
				node = new Node(sentinel_);
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

		//! Deallocates a node.
		/*!
		Time complexity:
		O(1)

		Exception safety:
		nothrow
		*/
		void deallocate(Node* node)
		{
			ASSERT(node);
			ASSERT(node != sentinel_);

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

		//! Turn all sentinel references to this tree.
		/*!
		Time complexity:
		O(size(node))

		Exception safety:
		nothrow
		*/
		void updateSentinel(Node* node)
		{
			ASSERT(!node->sentinel());

			for (integer i = 0;i < 2;++i)
			{
				Node*& childRef = node->childSet[i];

				if (childRef->sentinel())
				{
					// This is a sentinel reference,
					// but not necessary to this tree.
					
					// Turn the reference to this tree.
					childRef = sentinel_;
				}
				else
				{
					// This is a normal node reference.
					// Recurse.
					updateSentinel(childRef);
				}
			}
		}

		Node* sentinel_;
		Node* leftMost_;
		Node* root_;
		integer size_;
	};

}

#endif
