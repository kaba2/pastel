// Description: PointKdTree::Cursor class
// Detail: A cursor to traverse the nodes of a PointKdTree

#ifndef PASTEL_POINTKDTREE_CURSOR_H
#define PASTEL_POINTKDTREE_CURSOR_H

#include "pastel/geometry/pointkdtree.hpp"

namespace Pastel
{

	template <int N, typename Real, typename ObjectPolicy>
	class PointKdTree<N, Real, ObjectPolicy>::Cursor
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

		bool empty() const
		{
			return node_ == 0;
		}

		bool containsPoints() const
		{
			return !node_->empty();
		}

		ConstObjectIterator begin() const
		{
			PENSURE(node_);
			PENSURE(leaf());

			LeafNode* leafNode = (LeafNode*)node_;

			return leafNode->begin();
		}

		ConstObjectIterator end() const
		{
			PENSURE(node_);
			PENSURE(leaf());

			LeafNode* leafNode = (LeafNode*)node_;

			ConstObjectIterator iterEnd = leafNode->last();
			if (leafNode->objects() > 0)
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
			PENSURE(leaf());

			LeafNode* leafNode = (LeafNode*)node_;

			return leafNode->objects();
		}

		Real min() const
		{
			PENSURE(node_);
			PENSURE(!leaf());

			return ((SplitNode*)node_)->min();
		}

		Real max() const
		{
			PENSURE(node_);
			PENSURE(!leaf());

			return ((SplitNode*)node_)->max();
		}

		Real positiveMin() const
		{
			PENSURE(node_);
			PENSURE(!leaf());

			return ((SplitNode*)node_)->positiveMin();
		}

		Real negativeMax() const
		{
			PENSURE(node_);
			PENSURE(!leaf());

			return ((SplitNode*)node_)->negativeMax();
		}

		Real splitPosition() const
		{
			PENSURE(node_);
			PENSURE(!leaf());

			return ((SplitNode*)node_)->splitPosition();
		}

		integer splitAxis() const
		{
			PENSURE(node_);
			PENSURE(!leaf());

			return ((SplitNode*)node_)->splitAxis();
		}

		const Vector<N, Real>* splitDirection() const
		{
			PENSURE(node_);
			PENSURE(!leaf());
			
			return ((SplitNode*)node_)->splitDirection();
		}

		bool leaf() const
		{
			PENSURE(node_);

			return node_->leaf();
		}

		Cursor parent() const
		{
			PENSURE(node_);

			return Cursor(node_->parent());
		}

		Cursor positive() const
		{
			PENSURE(node_);
			PENSURE(!leaf());

			return Cursor(((SplitNode*)node_)->positive());
		}

		Cursor negative() const
		{
			PENSURE(node_);
			PENSURE(!leaf());

			return Cursor(((SplitNode*)node_)->negative());
		}

		Real projectedPosition(
			const Point<N, Real>& point) const
		{
			PENSURE(node_);
			PENSURE(!leaf());

			return ((SplitNode*)node_)->projectedPosition(point);
		}

	private:
		friend class PointKdTree<N, Real, ObjectPolicy>;

		explicit Cursor(Node* node)
			: node_(node)
		{
		}

		Node* node_;
	};

}

#endif
