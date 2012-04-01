#ifndef PASTEL_TREE_CURSOR_H
#define PASTEL_TREE_CURSOR_H

#include "pastel/sys/tree.h"
#include "pastel/sys/tree_node.h"
#include "pastel/sys/ensure.h"

namespace Pastel
{

	template <typename Tree_Options, typename Derived>
	class Tree;

	template <typename Data, int N>
	class Tree_Cursor;

	template <typename Data, int N>
	class Tree_ConstCursor
	{
	private:
		typedef Tree_Private::Node<Data, N> Node;
		friend class Tree_Cursor<Data, N>;
		
		template <typename Tree_Options, typename Derived>
		friend class Tree;

	public:
		// Using default copy constructor.
		// Using default assignment.
		// Using default destructor.

		Tree_ConstCursor()
			: node_(0)
		{
		}

		void swap(Tree_ConstCursor& that)
		{
			std::swap(node_, that.node_);
		}

		bool empty() const
		{
			return node_ == 0;
		}

		bool sentinel() const
		{
			// The sentinel node is the unique node in
			// which all children point to itself.
			return node_ == node_->childSet_[0];
		}

		Tree_Cursor<Data, N> parent() const
		{
			return Tree_Cursor<Data, N>(node_->parent);
		}

		Tree_Cursor<Data, N> child(integer index) const
		{
			ASSERT_OP(index, >=, 0);
			ASSERT_OP(index, <, N);

			return Tree_Cursor<Data, N>(node_->childSet[index]);
		}

		const Data& operator*() const
		{
			ASSERT(!empty());
			ASSERT(node_->data());

			return *(node_->data());
		}

		const Data* operator->() const
		{
			ASSERT(!empty());
			ASSERT(node_->data());

			return node_->data();
		}

	protected:
		explicit Tree_ConstCursor(const Node* node)
			: node_(node)
		{
		}

		const Node* node() const
		{
			return node_;
		}

		const Node* node_;
	};

	template <typename Data, int N>
	class Tree_Cursor
		: public Tree_ConstCursor<Data, N>
	{
	public:
		// Using default constructor.
		// Using default copy constructor.
		// Using default assignment.
		// Using default destructor.

		template <typename Tree_Options, typename Derived>
		friend class Tree;

		typedef Tree_ConstCursor<Data, N> Base;
		
		void swap(Tree_Cursor& that)
		{
			Base::swap(that);
		}

		typedef Base::Node Node;
		using Base::empty;
		using Base::sentinel;

		Tree_Cursor parent() const
		{
			return Tree_Cursor((Node*)Base::parent().node_);
		}
		
		Tree_Cursor child(integer index) const
		{
			return Tree_Cursor((Node*)Base::child(index).node_);
		}

		Data& operator*() const
		{
			return (Data&)Base::operator*();
		}

		Data* operator->() const
		{
			return (Data*)Base::operator->();
		}

	private:
		explicit Tree_Cursor(Node* node)
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
