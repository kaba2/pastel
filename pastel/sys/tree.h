// Description: A generic binary tree

#ifndef PASTEL_TREE_H
#define PASTEL_TREE_H

#include "pastel/sys/destruct.h"
#include "pastel/sys/tree_node.h"
#include "pastel/sys/tree_iterator.h"
#include "pastel/sys/bidirectionaliterator_range.h"

namespace Pastel
{

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
		Tree()
			: sentinel_(0)
			, leftMost_(0)
			, root_(0)
			, size_(0)
		{
			construct();
		}

		Tree(const Tree& that)
			: sentinel_(0)
			, leftMost_(0)
			, root_(0)
			, size_(0)
		{
			construct();

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
				throw;
			}
		}

		Tree(Tree&& that)
			: sentinel_(0)
			, leftMost_(0)
			, root_(0)
			, size_(0)
		{
			construct();

			swap(that);
		}

		~Tree()
		{
			clear();

			// The sentinel node should not be
			// destructed with deallocate(). It
			// has no user data.
			delete sentinel_;
			sentinel_ = 0;
		}

		Tree& operator=(const Tree& that)
		{
			Tree copy(that);
			swap(copy);
			return *this;
		}

		Tree& operator=(Tree&& that)
		{
			Tree copy(that);
			swap(copy);
			return *this;
		}

		void swap(Tree& that)
		{
			using std::swap;

			swap(sentinel_, that.sentinel_);
			swap(leftMost_, that.leftMost_);
			swap(root_, that.root_);
			swap(size_, that.size_);
		}

		bool empty() const
		{
			return size_ == 0;
		}

		integer size() const
		{
			return size_;
		}
		
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

		Iterator root()
		{
			return Iterator(root_);
		}

		ConstIterator root() const
		{
			return ConstIterator(root_);
		}

		ConstIterator croot() const
		{
			return ConstIterator(root_);
		}

		Iterator begin()
		{
			return Iterator(leftMost_);
		}

		ConstIterator begin() const
		{
			return ConstIterator(leftMost_);
		}

		ConstIterator cbegin() const
		{
			return ConstIterator(leftMost_);
		}

		Iterator end()
		{
			return Iterator(sentinel_);
		}

		ConstIterator end() const
		{
			return ConstIterator(sentinel_);
		}

		ConstIterator cend() const
		{
			return ConstIterator(sentinel_);
		}

		Range range()
		{
			return Range(begin(), end());
		}

		ConstRange range() const
		{
			return ConstRange(begin(), end());
		}

		ConstRange crange() const
		{
			return ConstRange(cbegin(), cend());
		}

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

		void insert(
			const ConstIterator& there, 
			integer childIndex,
			const Tree& that)
		{
			PENSURE_OP(childIndex, >=, 0);
			PENSURE_OP(childIndex, <, 2);
			
			integer rollback = 0;
			ConstIterator copyRoot;
			try
			{
				copyRoot = insert(there, childIndex, *that.root());
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
		}

		void insert(
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
			
			integer rollback = 0;
			ConstIterator copyRoot;
			try
			{

				Node* thatNode = (Node*)that.root().node_;
				Node* thereNode = (Node*)there.node_;

				if (thereNode != sentinel_)
				{
					thereNode->childSet[childIndex] = thatNode;
				}
				thatNode->parent = parent;

				if (thereNode == leftMost_)
				{
					leftMost_ = that.leftMost_;
				}
				
				Node*& rightMost = sentinel_->parent;
				if (thereNode == rightMost)
				{
					rightMost = that.rightMost_;
				}

				size_ += that.size_;

				that.leftMost_ = that.sentinel_;
				that.root_ = that.sentinel_;
				that.size_ = 0;
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
		}

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

	private:
		void construct()
		{
			// The sentinel node should not be
			// constructed with allocate(). It
			// has no user data.
			sentinel_ = new Node;

			leftMost_ = sentinel_;
			root_ = sentinel_;
		}

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

		Node* allocate(const Data& data)
		{
			Node* node = new Node(sentinel_);

			if (node->data())
			{
				new (node->data()) Data(data);
			}

			++size_;

			return node;
		}

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

		Node* sentinel_;
		Node* leftMost_;
		Node* root_;
		integer size_;
	};

}

#endif
