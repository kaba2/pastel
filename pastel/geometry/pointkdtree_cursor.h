// Detail: A cursor to traverse the nodes of a PointKdTree

#ifndef PASTEL_POINTKDTREE_CURSOR_H
#define PASTEL_POINTKDTREE_CURSOR_H

#include "pastel/geometry/pointkdtree.h"

namespace Pastel
{

	template <typename Real, int N, typename PointPolicy>
	class PointKdTree<Real, N, PointPolicy>::Cursor
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

		// Points

		ConstPointIterator begin() const
		{
			PENSURE(node_);
			return node_->first();
		}

		ConstPointIterator end() const
		{
			PENSURE(node_);

			ConstPointIterator iterEnd = node_->last();
			if (!empty())
			{
				++iterEnd;
			}

			return iterEnd;
		}

		ConstPointDataIterator pointBegin() const
		{
			return ConstPointDataIterator(begin());
		}

		ConstPointDataIterator pointEnd() const
		{
			return ConstPointDataIterator(end());
		}

		integer points() const
		{
			PENSURE(node_);
			return node_->points();
		}

		bool empty() const
		{
			PENSURE(node_);
			return node_->empty();
		}

		// Splitting plane

		const Real& splitPosition() const
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

		const Real& min() const
		{
			PENSURE(node_);
			return node_->min();
		}

		const Real& max() const
		{
			PENSURE(node_);
			return node_->max();
		}

		const Real& prevMin() const
		{
			PENSURE(node_);
			return node_->prevMin();
		}

		const Real& prevMax() const
		{
			PENSURE(node_);
			return node_->prevMax();
		}

		template <typename NormBijection>
		Real updateDistance(
			const Real& position,
			const Real& distance,
			const NormBijection& normBijection) const
		{
			PENSURE(node_);
			return node_->updateDistance(
				position,
				distance,
				normBijection);
		}

	private:
		friend class PointKdTree<Real, N, PointPolicy>;

		explicit Cursor(Node* node)
			: node_(node)
		{
		}

		Node* node_;
	};

}

#endif
