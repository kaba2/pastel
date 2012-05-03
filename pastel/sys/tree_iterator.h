#ifndef PASTEL_TREE_ITERATOR_H
#define PASTEL_TREE_ITERATOR_H

#include "pastel/sys/tree.h"
#include "pastel/sys/tree_node.h"
#include "pastel/sys/ensure.h"

#include <boost/operators.hpp>

namespace Pastel
{

	template <typename Type, typename Derived>
	class Tree;

	template <typename Data>
	class Tree_Iterator;

	template <typename Data>
	class Tree_ConstIterator
		: boost::bidirectional_iteratable<Tree_ConstIterator<Data>, const Data
		, boost::less_than_comparable<Tree_ConstIterator<Data>
		> >
	{
	private:
		typedef Tree_Private::Node Node;
		typedef Tree_Private::Data_Node<Data> Data_Node;
		friend class Tree_Iterator<Data>;
		
		template <typename Type, typename Derived>
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
			return node_ == 0;
		}

		bool sentinel() const
		{
			return node_->sentinel();
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
			ASSERT(!empty());
			PENSURE(!node_->sentinel());
			ASSERT(((Data_Node*)node_)->data());

			return *(((Data_Node*)node_)->data());
		}

		Tree_ConstIterator& operator++()
		{
			// If the cursor is in the sentinel node,
			// we should remain in that node.
			if (!node_->sentinel())
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
		
			if (child->sentinel())
			{
				result = node_;

				const Node* previous = 0;
				do
				{
					previous = result;
					result = previous->parent;
				}
				while(!result->sentinel() && 
					result->child(backward) != previous);
			}
			else
			{
				result = child;

				while(!result->child(backward)->sentinel())
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

		template <typename Type, typename Derived>
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
		using Base::sentinel;

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

	template <typename Type, typename Derived>
	Tree_Iterator<Type> begin(Tree<Type, Derived>& tree)
	{
		return tree.begin();
	}

	template <typename Type, typename Derived>
	Tree_ConstIterator<Type> begin(const Tree<Type, Derived>& tree)
	{
		return tree.begin();
	}

	template <typename Type, typename Derived>
	Tree_ConstIterator<Type> cbegin(Tree<Type, Derived>& tree)
	{
		return tree.cbegin();
	}

	template <typename Type, typename Derived>
	Tree_ConstIterator<Type> cbegin(const Tree<Type, Derived>& tree)
	{
		return tree.cbegin();
	}

	template <typename Type, typename Derived>
	Tree_Iterator<Type> end(Tree<Type, Derived>& tree)
	{
		return tree.end();
	}

	template <typename Type, typename Derived>
	Tree_ConstIterator<Type> end(const Tree<Type, Derived>& tree)
	{
		return tree.end();
	}

	template <typename Type, typename Derived>
	Tree_ConstIterator<Type> cend(Tree<Type, Derived>& tree)
	{
		return tree.cend();
	}

	template <typename Type, typename Derived>
	Tree_ConstIterator<Type> cend(const Tree<Type, Derived>& tree)
	{
		return tree.cend();
	}

}

#endif
