// Description: PointKdTree::Cursor class
// Detail: A cursor to traverse the nodes of a PointKdTree

#ifndef PASTEL_POINTKDTREE_CURSOR_H
#define PASTEL_POINTKDTREE_CURSOR_H

#include "pastel/geometry/pointkdtree.h"

namespace Pastel
{

	template <typename Real, int N, typename ObjectPolicy>
	class PointKdTree<Real, N, ObjectPolicy>::Cursor
		: boost::less_than_comparable<Cursor
		, boost::equality_comparable<Cursor
		> >
	{
	public:
		// Using default copy constructor.
		// Using default assignment.
		// Using default destructor.

		Cursor()
			: node_(0)
		{
		}

		bool operator<(const Cursor& that) const
		{
			return node_ < that.node_;
		}

		bool operator==(const Cursor& that) const
		{
			return node_ == that.node_;
		}

		bool exists() const
		{
			return node_ != 0;
		}

		// Tree

		Cursor parent() const
		{
			PENSURE(node_);
			return Cursor(node_->parent());
		}

		Cursor right() const
		{
			PENSURE(node_);
			return Cursor(node_->right());
		}

		Cursor left() const
		{
			PENSURE(node_);
			return Cursor(node_->left());
		}

		bool leaf() const
		{
			PENSURE(node_);
			return node_->leaf();
		}

		bool isBucket() const
		{
			PENSURE(node_);
			return node_->isBucket();
		}

		Cursor bucket() const
		{
			PENSURE(node_);
			PENSURE(leaf());
			return Cursor(node_->bucket());
		}

		// Objects

		ConstObjectIterator begin() const
		{
			PENSURE(node_);
			return node_->first();
		}

		ConstObjectIterator end() const
		{
			PENSURE(node_);

			ConstObjectIterator iterEnd = node_->last();
			if (!empty())
			{
				++iterEnd;
			}

			return iterEnd;
		}

		ConstObjectDataIterator objectBegin() const
		{
			return ConstObjectDataIterator(begin());
		}

		ConstObjectDataIterator objectEnd() const
		{
			return ConstObjectDataIterator(end());
		}

		integer objects() const
		{
			PENSURE(node_);
			return node_->objects();
		}

		bool empty() const
		{
			PENSURE(node_);
			return node_->empty();
		}

		// Splitting plane

		Real splitPosition() const
		{
			PENSURE(node_);
			return node_->splitPosition();
		}

		integer splitAxis() const
		{
			PENSURE(node_);
			return node_->splitAxis();
		}

		// Bounds

		Real min() const
		{
			PENSURE(node_);
			return node_->min();
		}

		Real max() const
		{
			PENSURE(node_);
			return node_->max();
		}

	private:
		friend class PointKdTree<Real, N, ObjectPolicy>;

		explicit Cursor(Node* node)
			: node_(node)
		{
		}

		Node* node_;
	};

}

#endif
