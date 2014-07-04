// Description: Point kd-tree cursor

#ifndef PASTELGEOMETRY_POINTKDTREE_CURSOR_H
#define PASTELGEOMETRY_POINTKDTREE_CURSOR_H

#include "pastel/geometry/pointkdtree.h"
#include "pastel/geometry/pointkdtree_fwd.h"

namespace Pastel
{

	template <typename Settings>
	class PointKdTree_Fwd<Settings>::Cursor
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

		Point_ConstIterator begin() const
		{
			PENSURE(node_);

			return node_->first();
		}

		Point_ConstIterator end() const
		{
			PENSURE(node_);

			Point_ConstIterator iterEnd = node_->last();
			if (!empty())
			{
				++iterEnd;
			}

			return iterEnd;
		}

		PointData_ConstIterator pointBegin() const
		{
			return PointData_ConstIterator(begin());
		}

		PointData_ConstIterator pointEnd() const
		{
			return PointData_ConstIterator(end());
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

		// FIX: This should be private!
		// Moved here due to a bug in gcc 4.0.
		explicit Cursor(Node* node)
			: node_(node)
		{
		}

	private:
		template <typename, template <typename> class>
		friend class PointKdTree;

		Node* node_;
	};

}

#endif
