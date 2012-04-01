// Description: Generic tree data structure

#ifndef PASTEL_TREE_H
#define PASTEL_TREE_H

#include "pastel/sys/tree_options_concept.h"
#include "pastel/sys/tree_node.h"
#include "pastel/sys/tree_iterator.h"
#include "pastel/sys/tree_cursor.h"

#include "pastel/sys/destruct.h"

#include <array>

namespace Pastel
{

	template <typename Type>
	class Binary_Tree_Options
	{
	public:
		enum {N = 2};

		typedef Type Data;
	};

	template <typename Tree_Options, typename Derived = EmptyClass>
	class Tree
	{
	public:
		typedef Tree_Options Options;
		enum {N = Options::N};
		typedef typename Options::Data Data;

		typedef Tree_Cursor<Data, N> Cursor;
		typedef Tree_ConstCursor<Data, N> ConstCursor;

	private:
		typedef Tree_Private::Node<Data, N> Node;

	public:
		Tree()
			: sentinel_(0)
			, size_(0)
		{
			construct();
		}

#		ifdef PASTEL_MOVE_SEMANTICS
		Tree(Tree&& that)
			: sentinel_(0)
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
			size_ = 0;
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
		}

		Cursor sentinel()
		{
			return Cursor(sentinel_);
		}

		ConstCursor sentinel() const
		{
			return ConstCursor(sentinel_);
		}

		Cursor insert(
			const ConstCursor& here, 
			integer childIndex,
			const Data& data = Data())
		{
			PENSURE_OP(childIndex, >=, 0);
			PENSURE_OP(childIndex, <, N);

			Node* parent = (Node*)here.node_;

			Node* node = allocate(data);
			node->parent = parent;

			Node*& child = parent->childSet[childIndex];
			
			const bool childAlreadyExists = (child != 0);
			PENSURE(!childAlreadyExists);

			if (parent != sentinel_)
			{
				parent->childSet[childIndex] = node;
			}
						
			return Cursor(node);
		}

		Tree detach(const ConstCursor& that)
		{
			ENSURE(!that.sentinel());

			Node* node = (Node*)that.node_;
			const integer detachedSize = size(node);

			// Detach the subtree given by 'that' 
			// from this tree.

			Node* parent = node->parent;
			integer index = 0;
			while (index < N && parent->childSet[index] != node)
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
		}

		integer size(Node* node)
		{
			integer result = 0;

			for (integer i = 0;i < N;++i)
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
			for (integer i = 0;i < N;++i)
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
		integer size_;
	};

}

#endif
