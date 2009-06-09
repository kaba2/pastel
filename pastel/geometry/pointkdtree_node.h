#ifndef PASTEL_POINTKDTREE_NODE_H
#define PASTEL_POINTKDTREE_NODE_H

#include "pastel/geometry/pointkdtree.hpp"

namespace Pastel
{

	template <int N, typename Real, typename ObjectPolicy>
	class PointKdTree<N, Real, ObjectPolicy>::Node
	{
	public:
		explicit Node(pointer_integer unknown)
			: unknown_(unknown)
		{
		}

		bool leaf() const
		{
			return ((unknown_ & 1) != 0);
		}

		//protected:

		// The first bit of 'unknown_' tells
		// if the node is a LeafNode (1) or an
		// IntermediateNode (0).
		// If the node is a LeafNode, then
		// the number of objects is encoded in
		// (unknown_ >> 1).
		// Otherwise unknown_ ^ (unknown_ & 0x3)
		// contains a pointer to the positive child node.
		// This trick relies on 4 byte alignment of nodes.

		pointer_integer unknown_;
	};

	template <int N, typename Real, typename ObjectPolicy>
	class PointKdTree<N, Real, ObjectPolicy>::LeafNode
		: public Node
	{
	public:
		// TODO: possible compiler bug: doesn't work without this.
		using Node::unknown_;

		LeafNode(
			const ObjectIterator& begin,
			const ObjectIterator& last,
			integer objects)
			: Node((objects << 1) + 1)
			, begin_(begin)
			, last_(last)
		{
		}

		void setBegin(const ObjectIterator& begin)
		{
			begin_ = begin;
		}

		ObjectIterator begin() const
		{
			return begin_;
		}

		void setLast(const ObjectIterator& last)
		{
			last_ = last;
		}

		ObjectIterator last() const
		{
			return last_;
		}

		void setObjects(integer objects)
		{
			unknown_ = (objects << 1) + 1;
		}

		integer objects() const
		{
			return unknown_ >> 1;
		}

	private:
		ObjectIterator begin_;
		ObjectIterator last_;
	};

	template <int N, typename Real, typename ObjectPolicy>
	class PointKdTree<N, Real, ObjectPolicy>::IntermediateNode_BspTree
		: public Node
	{
	private:
		// TODO: possible compiler bug: doesn't work without this.
		using Node::unknown_;

	public:
		IntermediateNode_BspTree(
			Node* positive,
			Node* negative,
			const Real& splitPosition,
			integer splitAxis,
			const Vector<N, Real>* splitDirection,
			const Real& min,
			const Real& max,
			const Real& positiveMin,
			const Real& negativeMax)
			: Node((pointer_integer)positive)
			, negative_(negative)
			, splitPosition_(splitPosition)
			, splitAxis_(splitAxis)
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
			return splitAxis_;
		}

		Node* positive() const
		{
			return (Node*)unknown_;
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

		const Vector<N, Real>* splitDirection() const
		{
			return &splitDirection_;
		}

		Real projectedPosition(
			const Point<N, Real>& point) const
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
		Node* negative_;
		Real splitPosition_;
		int32 splitAxis_;
		Real min_;
		Real max_;
		Real positiveMin_;
		Real negativeMax_;
		Vector<N, Real> splitDirection_;
	};

	template <int N, typename Real, typename ObjectPolicy>
	class PointKdTree<N, Real, ObjectPolicy>::IntermediateNode_KdTree
		: public Node
	{
	private:
		// TODO: possible compiler bug: doesn't work without this.
		using Node::unknown_;

	public:
		IntermediateNode_KdTree(
			Node* positive,
			Node* negative,
			const Real& splitPosition,
			integer splitAxis,
			const Vector<N, Real>* splitDirection,
			const Real& min,
			const Real& max,
			const Real& positiveMin,
			const Real& negativeMax)
			: Node((pointer_integer)positive)
			, negative_(negative)
			, splitPosition_(splitPosition)
			, splitAxis_(splitAxis)
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
			return splitAxis_;
		}

		Node* positive() const
		{
			return (Node*)unknown_;
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

		const Vector<N, Real>* splitDirection() const
		{
			return 0;
		}

		Real projectedPosition(
			const Point<N, Real>& point) const
		{
			return point[splitAxis_];
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
		Node* negative_;
		Real splitPosition_;
		int32 splitAxis_;
		Real min_;
		Real max_;
	};

}

#endif
