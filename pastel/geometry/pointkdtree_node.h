#ifndef PASTEL_POINTKDTREE_NODE_H
#define PASTEL_POINTKDTREE_NODE_H

#include "pastel/geometry/pointkdtree.h"

namespace Pastel
{

	template <typename Real, int N, typename PointPolicy>
	class PointKdTree<Real, N, PointPolicy>::Node
	{
	public:
		Node(
			Node* parent,
			Node* right,
			Node* left,
			const ConstPointIterator& first,
			const ConstPointIterator& last,
			integer points,
			integer splitAxis,
			const Real& splitPosition)
			: parent_(parent)
			, right_(right)
			, left_(left)
			, first_(first)
			, last_(last)
			, points_(points)
			, splitAxis_(splitAxis)
			, splitPosition_(splitPosition)
			, min_(infinity<Real>())
			, max_(-infinity<Real>())
		{
			ASSERT_OP(points, >=, 0);
			ASSERT_OP(splitAxis, >=, 0);
		}

		// Tree

		Node* parent() const
		{
			return parent_;
		}

		void setRight(Node* right)
		{
			right_ = right;
		}

		Node* right() const
		{
			return right_;
		}

		void setLeft(Node* left)
		{
			left_ = left;
		}

		Node* left() const
		{
			return left_;
		}

		bool leaf() const
		{
			return left_ == 0;
		}

		// Points

		void setFirst(const ConstPointIterator& first)
		{
			first_ = first;
		}

		const ConstPointIterator& first() const
		{
			return first_;
		}

		void setLast(const ConstPointIterator& last)
		{
			last_ = last;
		}

		const ConstPointIterator& last() const
		{
			return last_;
		}

		ConstPointIterator end() const
		{
			ConstPointIterator result = last_;
			if (points_ > 0)
			{
				++result;
			}

			return result;
		}

		void setPoints(integer points)
		{
			ASSERT_OP(points, >=, 0);

			points_ = points;
		}

		integer points() const
		{
			return points_;
		}

		bool empty() const
		{
			return points_ == 0;
		}

		void erase(
			const ConstPointIterator& iter,
			const ConstPointIterator& end)
		{
			if (iter == first_)
			{
				if (iter == last_)
				{
					first_ = end;
					last_ = end;
				}
				else
				{
					++first_;
				}
			}
			else if (iter == last_)
			{
				--last_;
			}
			--points_;
		}

		void insert(
			const ConstPointIterator& first,
			const ConstPointIterator& last,
			integer points,
			const ConstPointIterator& end)
		{
			last_ = last;

			if (first_ == end)
			{
				// If there are currently no
				// points in the node, set the 'last' iterator.
				first_ = first;
			}

			/*
			first_ = first;

			if (last_ == end)
			{
				// If there are currently no
				// points in the node, set the 'last' iterator.
				last_ = last;
			}
			*/

			points_ += points;
		}

		// Splitting plane

		void setSplitAxis(integer splitAxis)
		{
			splitAxis_ = splitAxis;
		}

		integer splitAxis() const
		{
			return splitAxis_;
		}

		void setSplitPosition(const Real& splitPosition)
		{
			splitPosition_ = splitPosition;
		}

		const Real& splitPosition() const
		{
			return splitPosition_;
		}

		// Bounds

		void setMin(const Real& min)
		{
			min_ = min;
		}

		const Real& min() const
		{
			return min_;
		}

		void setMax(const Real& max)
		{
			max_ = max;
		}

		const Real& max() const
		{
			return max_;
		}

		void setPrevMin(const Real& prevMin)
		{
			prevMin_ = prevMin;
		}

		const Real& prevMin() const
		{
			return prevMin_;
		}

		void setPrevMax(const Real& prevMax)
		{
			prevMax_ = prevMax;
		}

		const Real& prevMax() const
		{
			return prevMax_;
		}

	private:
		// Tree

		Node* parent_;
		Node* right_;
		Node* left_;

		// Points

		ConstPointIterator first_;
		ConstPointIterator last_;
		uint32 points_;

		// Splitting plane

		uint32 splitAxis_;
		Real splitPosition_;

		// Bounds of the contained
		// points on the splitting
		// axis of the _parent_ node.

		Real min_;
		Real max_;

		// Previous values of min_ and max_ 
		// on the splitting axis.

		Real prevMin_;
		Real prevMax_;
	};

}

#endif
