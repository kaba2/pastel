#ifndef PASTEL_POINTKDTREE_NODE_H
#define PASTEL_POINTKDTREE_NODE_H

#include "pastel/geometry/pointkdtree.hpp"

namespace Pastel
{

	template <int N, typename Real, typename ObjectPolicy>
	class PointKdTree<N, Real, ObjectPolicy>::Node
	{
	public:
		explicit Node(
			SplitNode* parent,
			pointer_integer data,
			bool isLeaf,
			bool isEmpty)
			: parent_(parent)
			, unknown_(0)
		{
			setUnknown(data, isLeaf, isEmpty);
		}

		SplitNode* parent() const
		{
			return parent_;
		}

		bool leaf() const
		{
			return ((unknown_ & 1) != 0);
		}

		bool empty() const
		{
			return ((unknown_ & 2) != 0);
		}

		void setEmpty()
		{
			if ((unknown_ & 2) == 0)
			{
				unknown_ += 2;
			}
		}

		void setNonEmpty()
		{
			if ((unknown_ & 2) == 2)
			{
				unknown_ -= 2;
			}
		}

	protected:
		// The first bit of 'unknown_' tells
		// if the node is a LeafNode (1) or an
		// SplitNode (0).
		// The second bit of 'unknown_' tells
		// if the subtree under the node is
		// void of points (1) or not (0).

		// If the node is a LeafNode, then
		// the number of objects is encoded in
		// (unknown_ >> 2).
		// Otherwise unknown_ ^ (unknown_ & 0x3)
		// contains a pointer to the positive child node.
		// This trick relies on 4 byte alignment of nodes.

		void setUnknown(
			pointer_integer data,
			bool isLeaf,
			bool isEmpty)
		{
			unknown_ = data;

			if (isLeaf)
			{
				unknown_ += 1;
			}

			if (isEmpty)
			{
				unknown_ += 2;
			}
		}

		SplitNode* parent_;
		pointer_integer unknown_;
	};

	template <int N, typename Real, typename ObjectPolicy>
	class PointKdTree<N, Real, ObjectPolicy>::LeafNode
		: public Node
	{
	private:
		using Node::setUnknown;
		// Possible compiler bug: using needed here, although
		// there is no need.
		using Node::unknown_;

	public:

		LeafNode(
			SplitNode* parent,
			const ConstObjectIterator& begin,
			const ConstObjectIterator& last,
			integer objects)
			: Node(parent, objects << 2, true, objects == 0)
			, begin_(begin)
			, last_(last)
		{
		}

		void setBegin(const ConstObjectIterator& begin)
		{
			begin_ = begin;
		}

		ConstObjectIterator begin() const
		{
			return begin_;
		}

		void setLast(const ConstObjectIterator& last)
		{
			last_ = last;
		}

		ConstObjectIterator last() const
		{
			return last_;
		}

		void setObjects(integer objects)
		{
			ASSERT_OP(objects, >=, 0);

			setUnknown(objects << 2, true, objects == 0);
		}

		integer objects() const
		{
			return unknown_ >> 2;
		}

		void erase(
			const ConstObjectIterator& iter,
			const ConstObjectIterator& end)
		{
			if (iter == begin_ &&
				iter == last_)
			{
				begin_ = end;
				last_ = end;
			}
			else if (iter == begin_)
			{
				++begin_;
			}
			else if (iter == last_)
			{
				--last_;
			}

			setObjects(objects() - 1);
		}

	private:
		ConstObjectIterator begin_;
		ConstObjectIterator last_;
	};

	template <int N, typename Real, typename ObjectPolicy>
	class PointKdTree<N, Real, ObjectPolicy>::SplitNode_BspTree
		: public Node
	{
	private:
		// Possible compiler bug: using needed here, although
		// there is no need.
		using Node::unknown_;

	public:
		SplitNode_BspTree(
			SplitNode* parent,
			Node* positive,
			Node* negative,
			const Real& splitPosition,
			integer splitAxis,
			const Vector<Real, N>* splitDirection,
			const Real& min,
			const Real& max,
			const Real& positiveMin,
			const Real& negativeMax,
			bool isEmpty)
			: Node(parent, splitAxis_ << 2, false, isEmpty)
			, positive_(positive)
			, negative_(negative)
			, splitPosition_(splitPosition)
			, splitDirection_(*splitDirection)
			, min_(min)
			, max_(max)
			, positiveMin_(positiveMin)
			, negativeMax_(negativeMax)
		{
		}

		const Real& splitPosition() const
		{
			return splitPosition_;
		}

		integer splitAxis() const
		{
			return (unknown_ >> 2);
		}

		Node* positive() const
		{
			return positive_;
		}

		Node* negative() const
		{
			return negative_;
		}

		const Real& min() const
		{
			return min_;
		}

		const Real& max() const
		{
			return max_;
		}

		Real positiveMin() const
		{
			return positiveMin_;
		}

		Real negativeMax() const
		{
			return negativeMax_;
		}

		const Vector<Real, N>* splitDirection() const
		{
			return &splitDirection_;
		}

		Real projectedPosition(
			const Point<Real, N>& point) const
		{
			return dot(asVector(point), splitDirection_);
		}

		void setMin(const Real& min)
		{
			min_ = min;
		}

		void setMax(const Real& max)
		{
			max_ = max;
		}

	private:
		Node* positive_;
		Node* negative_;
		Real splitPosition_;
		Real min_;
		Real max_;
		Real positiveMin_;
		Real negativeMax_;
		Vector<Real, N> splitDirection_;
	};

	template <int N, typename Real, typename ObjectPolicy>
	class PointKdTree<N, Real, ObjectPolicy>::SplitNode_KdTree
		: public Node
	{
	private:
		// Possible compiler bug: using needed here, although
		// there is no need.
		using Node::unknown_;

	public:
		SplitNode_KdTree(
			SplitNode* parent,
			Node* positive,
			Node* negative,
			const Real& splitPosition,
			integer splitAxis,
			const Vector<Real, N>* splitDirection,
			const Real& min,
			const Real& max,
			const Real& positiveMin,
			const Real& negativeMax,
			bool isEmpty)
			: Node(parent, splitAxis << 2, false, isEmpty)
			, positive_(positive)
			, negative_(negative)
			, splitPosition_(splitPosition)
			, min_(min)
			, max_(max)
		{
			ASSERT(positiveMin == splitPosition);
			ASSERT(negativeMax == splitPosition);
		}

		const Real& splitPosition() const
		{
			return splitPosition_;
		}

		integer splitAxis() const
		{
			return (unknown_ >> 2);
		}

		Node* positive() const
		{
			return positive_;
		}

		Node* negative() const
		{
			return negative_;
		}

		Real min() const
		{
			return min_;
		}

		Real max() const
		{
			return max_;
		}

		Real positiveMin() const
		{
			return splitPosition_;
		}

		Real negativeMax() const
		{
			return splitPosition_;
		}

		const Vector<Real, N>* splitDirection() const
		{
			return 0;
		}

		Real projectedPosition(
			const Point<Real, N>& point) const
		{
			return point[splitAxis()];
		}

		void setMin(const Real& min)
		{
			min_ = min;
		}

		void setMax(const Real& max)
		{
			max_ = max;
		}

	private:
		Node* positive_;
		Node* negative_;
		Real splitPosition_;
		Real min_;
		Real max_;
	};

}

#endif
