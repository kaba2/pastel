#ifndef PASTEL_TREE_ITERATOR_H
#define PASTEL_TREE_ITERATOR_H

#include "pastel/sys/tree.h"
#include "pastel/sys/tree_node.h"
#include "pastel/sys/ensure.h"

#include <boost/operators.hpp>

namespace Pastel
{

	template <typename Type>
	class Tree;

	template <typename Data>
	class Tree_Iterator;

	template <typename Data>
	class Tree_ConstIterator
		: public boost::bidirectional_iteratable<Tree_ConstIterator<Data>, const Data*
		, boost::less_than_comparable<Tree_ConstIterator<Data>
		> >
	{
	private:
		typedef Tree_::Node Node;
		typedef Tree_::Data_Node<Data> Data_Node;
		friend class Tree_Iterator<Data>;
		
		template <typename Type>
		friend class Tree;

	public:
		typedef const Data value_type;
		typedef pointer_integer difference_type;
		typedef const Data* pointer;
		typedef const Data& reference;
		typedef std::bidirectional_iterator_tag iterator_category;

		// Using default copy constructor.
		// Using default assignment.
		// Using default destructor.

		Tree_ConstIterator()
			: node_(0)
		{
		}

		void swap(Tree_ConstIterator& that)
		{
			std::swap(node_, that.node_);
		}

		bool empty() const
		{
			ASSERT(node_);
			return node_->empty();
		}

		Tree_ConstIterator parent() const
		{
			return Tree_ConstIterator(node_->parent);
		}

		Tree_ConstIterator child(integer index) const
		{
			ASSERT_OP(index, >=, 0);
			ASSERT_OP(index, <, 2);

			return Tree_ConstIterator(node_->child(index));
		}

		const Data& operator*() const
		{
			ASSERT(node_);
			PENSURE(!node_->empty());
			ASSERT(((Data_Node*)node_)->data());

			return *(((Data_Node*)node_)->data());
		}

		Tree_ConstIterator& operator++()
		{
			// If the cursor is in the sentinel node,
			// we should remain in that node.
			if (!node_->empty())
			{
				node_ = next(1);
			}

			return *this;
		}

		Tree_ConstIterator& operator--()
		{
			// If the cursor is in the sentinel node,
			// we should back out from it to the
			// 'maximum' node (the parent of the
			// sentinel node). Thus no special 
			// handling for the sentinel node here.

			node_ = next(0);

			return *this;
		}

		bool operator==(const Tree_ConstIterator& that) const
		{
			return node_ == that.node_;
		}

		bool operator<(const Tree_ConstIterator& that) const
		{
			return node_ < that.node_;
		}

		integer children() const
		{
			integer result = 0;
			for (integer i = 0;i < 2;++i)
			{
				if (!child(i).empty())
				{
					++result;
				}
			}
			
			return result;
		}

	protected:
		explicit Tree_ConstIterator(const Node* node)
			: node_(node)
		{
		}

		const Node* node() const
		{
			return node_;
		}

		const Node* next(integer forward)
		{
			ASSERT_OP(forward, >=, 0);
			ASSERT_OP(forward, <=, 1);

			const integer backward = !forward;

			const Node* result;
			const Node* child = node_->child(forward);
		
			if (child->empty())
			{
				result = node_;

				const Node* previous = 0;
				do
				{
					previous = result;
					result = previous->parent;
				}
				while(!result->empty() && 
					result->child(backward) != previous);
			}
			else
			{
				result = child;

				while(!result->child(backward)->empty())
				{
					result = result->child(backward);
				}
			}

			return result;
		}

		const Node* node_;
	};

	template <typename Data>
	class Tree_Iterator
		: public Tree_ConstIterator<Data>
	{
	public:
		// Using default constructor.
		// Using default copy constructor.
		// Using default assignment.
		// Using default destructor.

		template <typename Type>
		friend class Tree;

		typedef Tree_ConstIterator<Data> Base;

		Tree_Iterator()
		{
		}
		
		void swap(Tree_Iterator& that)
		{
			Base::swap(that);
		}

		typedef Base::Node Node;
		using Base::empty;

		Tree_Iterator parent() const
		{
			return Tree_Iterator((Node*)Base::parent().node_);
		}
		
		Tree_Iterator child(integer index) const
		{
			return Tree_Iterator((Node*)Base::child(index).node_);
		}

		Data& operator*() const
		{
			return (Data&)Base::operator*();
		}

		Data* operator->() const
		{
			return (Data*)Base::operator->();
		}

		Tree_Iterator& operator++()
		{
			Base::operator++();

			return *this;
		}

		Tree_Iterator& operator--()
		{
			Base::operator--();

			return *this;
		}

		bool operator==(const Tree_Iterator& that) const
		{
			return Base::operator==(that);
		}

		bool operator!=(const Tree_Iterator& that) const
		{
			return Base::operator!=(that);
		}

		bool operator<(const Tree_Iterator& that) const
		{
			return Base::operator<(that);
		}

		bool operator<=(const Tree_Iterator& that) const
		{
			return Base::operator<=(that);
		}

		bool operator>(const Tree_Iterator& that) const
		{
			return Base::operator>(that);
		}

		bool operator>=(const Tree_Iterator& that) const
		{
			return Base::operator>=(that);
		}

	private:
		explicit Tree_Iterator(Node* node)
			: Base(node)
		{
		}

		Node* node() const
		{
			return (Node*)Base::node_;
		}
	};

}

#endif
