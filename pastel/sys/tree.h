// Description: Generic tree data structure

#ifndef PASTEL_TREE_H
#define PASTEL_TREE_H

#include "pastel/sys/destruct.h"
#include "pastel/sys/tree_node.h"
#include "pastel/sys/tree_iterator.h"

namespace Pastel
{

	template <typename Type, typename Derived = EmptyClass>
	class Tree
	{
	public:
		typedef Type Data;

		typedef Tree_Iterator<Data> Iterator;
		typedef Tree_ConstIterator<Data> ConstIterator;

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

#		ifdef PASTEL_MOVE_SEMANTICS
		Tree(Tree&& that)
			: sentinel_(0)
			, leftMost_(0)
			, root_(0)
			, size_(0)
		{
			construct();

			swap(that);
		}
#		endif

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

#		ifdef PASTEL_MOVE_SEMANTICS
		Tree& operator=(Tree&& that)
		{
			Tree copy(that);
			swap(copy);
			return *this;
		}
#		endif

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
			clear(sentinel_);

			leftMost_ = sentinel_;
			root_ = sentinel_;
			size_ = 0;
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

		Iterator insert(
			const ConstIterator& here, 
			integer childIndex,
			const Data& data = Data())
		{
			PENSURE_OP(childIndex, >=, 0);
			PENSURE_OP(childIndex, <, 2);

			Node* parent = (Node*)here.node_;

			Node* node = allocate(data);
			node->parent = parent;

			Node*& parentChild = parent->childSet[childIndex];
			
			const bool childAlreadyExists = (parentChild != 0);
			PENSURE(!childAlreadyExists);

			// The rightmost node is stored at the
			// sentinel parent link. 
			Node*& rightMost = sentinel_->parent;

			if (parent != sentinel_)
			{
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
			}
			else
			{
				// The children of the sentinel node always
				// refer to itself. Thus we do not set the
				// child pointer here.
				
				// Since this is the first actual node in
				// the tree, it is leftmost, rightmost and
				// the root.
				leftMost_ = node;
				rightMost = node;
				root_ = node;
			}

			return Iterator(node);
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

		void clear(Node* node)
		{
			for (integer i = 0;i < 2;++i)
			{
				Node* child = node->childSet[i];
				if (child != sentinel_)
				{
					clear(child);
				}
			}
			
			if (node != sentinel_)
			{
				deallocate(node);
			}
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

		Tree(const Tree& that);

		Node* sentinel_;
		Node* leftMost_;
		Node* root_;
		integer size_;
	};

}

#endif
