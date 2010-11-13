#ifndef PASTEL_POINTKDTREE_PRIVATE_HPP
#define PASTEL_POINTKDTREE_PRIVATE_HPP

#include "pastel/geometry/pointkdtree.h"
#include "pastel/geometry/longestmedian_splitrule_pointkdtree.h"

#include <boost/type_traits/has_trivial_destructor.hpp>

namespace Pastel
{

	// Private

	template <typename Real, int N, typename PointPolicy>
	void PointKdTree<Real, N, PointPolicy>::initialize()
	{
		root_ = allocateLeaf(
			0,
			pointList_.end(), 
			pointList_.end(),
			0);

		++leaves_;
	}

	template <typename Real, int N, typename PointPolicy>
	void PointKdTree<Real, N, PointPolicy>::copyConstruct(
		Node* thisNode, Node* thatNode)
	{
		ASSERT(thisNode);
		ASSERT(thatNode);

		if (!thatNode->leaf())
		{
			ASSERT(thisNode->leaf());

			subdivide(
				thisNode, 
				thatNode->splitPosition(),
				thatNode->splitAxis());

			copyConstruct(
				thisNode->left(),
				thatNode->left());

			copyConstruct(
				thisNode->right(),
				thatNode->right());

			thisNode->setMin(
				thatNode->min());
			thisNode->setMax(
				thatNode->max());
			thisNode->updateHierarchical();
		}
	}

	template <typename Real, int N, typename PointPolicy>
	typename PointKdTree<Real, N, PointPolicy>::Node*
		PointKdTree<Real, N, PointPolicy>::allocateLeaf(
		Node* parent,
		const Point_ConstIterator& first,
		const Point_ConstIterator& last,
		integer points)
	{
		// A null pointer in 'left' is used to identify a leaf 
		// node.

		Node* node = (Node*)nodeAllocator_.allocate();

		new(node) Node(
			parent,
			0,
			0,
			first,
			last,
			points, 
			0, 0);

		return node;
	}

	template <typename Real, int N, typename PointPolicy>
	void PointKdTree<Real, N, PointPolicy>::destructSubtree(
		Node* node)
	{
		if (!boost::has_trivial_destructor<Real>())
		{
			if (!node->leaf())
			{
				destructSubtree(node->left());
				destructSubtree(node->right());
			}

			StdExt::destruct(node);
		}
	}

	template <typename Real, int N, typename PointPolicy>
	AlignedBox<Real, N> PointKdTree<Real, N, PointPolicy>::computeBound(
		const Point_ConstIterator& begin, 
		const Point_ConstIterator& end) const
	{
		AlignedBox<Real, N> bound(
			ofDimension(dimension()));

		Point_ConstIterator iter = begin;
		const Point_ConstIterator iterEnd = end;
		while(iter != iterEnd)
		{
			extendToCover(
				evaluate(pointPolicy_(iter->point())), 
				bound);
			++iter;
		}
		
		return bound;
	}

	template <typename Real, int N, typename PointPolicy>
	std::pair<Real, Real> PointKdTree<Real, N, PointPolicy>::computeBound(
		const Point_ConstIterator& begin, 
		const Point_ConstIterator& end,
		integer axis) const
	{
		std::pair<Real, Real> bound(
			infinity<Real>(), -infinity<Real>());

		Point_ConstIterator iter = begin;
		const Point_ConstIterator iterEnd = end;
		while(iter != iterEnd)
		{
			const Real position = 
				pointPolicy_(iter->point())[axis];
			if (position < bound.first)
			{
				bound.first = position;
			}
			if (position > bound.second)
			{
				bound.second = position;
			}

			++iter;
		}
		
		return bound;
	}

	template <typename Real, int N, typename PointPolicy>
	void PointKdTree<Real, N, PointPolicy>::merge(
		Node* node)
	{
		ASSERT(node);
		
		if (!node->leaf())
		{
			if (node == root_)
			{
				merge();
			}
			else
			{
				erase(node->left());
				erase(node->right());

				node->setLeft(0);
				node->setRight(0);
			}
		}
	}

	template <typename Real, int N, typename PointPolicy>
	void PointKdTree<Real, N, PointPolicy>::erase(
		Node* node)
	{
		ASSERT(node);

		if (!node->leaf())
		{
			erase(node->left());
			erase(node->right());
		}
		else
		{
			--leaves_;
		}

		if (!boost::has_trivial_destructor<Real>())
		{
			StdExt::destruct(node);
		}

		nodeAllocator_.deallocate(node);
	}

	template <typename Real, int N, typename PointPolicy>
	void PointKdTree<Real, N, PointPolicy>::setLeaf(
		const Point_ConstIterator& begin,
		const Point_ConstIterator& end,
		Node* node)
	{
		Point_ConstIterator iter = begin;
		while(iter != end)
		{
			iter->setLeaf(node);
			++iter;
		}
	}

	template <typename Real, int N, typename PointPolicy>
	void PointKdTree<Real, N, PointPolicy>::updateBounds(
		Node* node, const AlignedBox<Real, N>& bound)
	{
		// We only allow to expand the
		// point bound.
		if (node->parent())
		{
			const integer splitAxis = node->parent()->splitAxis();
			node->setMin(std::min(bound.min()[splitAxis], node->min()));
			node->setMax(std::max(bound.max()[splitAxis], node->max()));
		}
	}

	template <typename Real, int N, typename PointPolicy>
	void PointKdTree<Real, N, PointPolicy>::updateHierarchicalUpwards(
		Node* node)
	{
		ASSERT(node);

		node = node->parent();
		while (node)
		{
			node->updateHierarchical();
			node = node->parent();
		}
	}

	template <typename Real, int N, typename PointPolicy>
	void PointKdTree<Real, N, PointPolicy>::erasePoints(
		Node* node)
	{
		ASSERT(node);

		// Actually remove the points.
		Point_ConstIterator iter = node->first();
		const Point_ConstIterator iterEnd = node->end();
		while(iter != iterEnd)
		{
			iter = pointList_.erase(iter);
		}

		// Clear the point ranges in the subtree.
		clearPoints(node);

		// Update hierarchical information.
		updateHierarchicalUpwards(node);
	}

	template <typename Real, int N, typename PointPolicy>
	void PointKdTree<Real, N, PointPolicy>::clearPoints(
		Node* node)
	{
		ASSERT(node);

		// Update the node state.

		node->setFirst(pointList_.end());
		node->setLast(pointList_.end());
		node->setPoints(0);
		node->setMin(infinity<Real>());
		node->setMax(-infinity<Real>());

		if (!node->leaf())
		{
			// Recurse deeper.

			clearPoints(node->right());
			clearPoints(node->left());
		}
	}

	template <typename Real, int N, typename PointPolicy>
	void PointKdTree<Real, N, PointPolicy>::subdivide(
		Node* node,
		const Real& splitPosition, 
		integer splitAxis,
		const Real& prevMin,
		const Real& prevMax)
	{
		ASSERT(node);
		ASSERT(node->leaf());
		ASSERT2(splitAxis >= 0 && splitAxis < dimension(), splitAxis, dimension());

		const integer points = node->points();
		const Point_ConstIterator nodeEnd = node->end();

		// Reorder the points along the split position.

		const SplitPredicate splitPredicate(
			splitPosition, splitAxis, 
			pointPolicy_);

		const std::pair<std::pair<Point_Iterator, integer>,
			std::pair<Point_Iterator, integer> > result =
			partition(pointList_, node->first(), nodeEnd,
			splitPredicate);

		Point_ConstIterator leftFirst = pointList_.end();
		Point_ConstIterator leftLast = pointList_.end();

		const integer leftPoints = result.first.second;
		if (leftPoints > 0)
		{
			leftFirst = result.first.first;
			leftLast = result.second.first;
			--leftLast;
		}

		Point_ConstIterator rightFirst = pointList_.end();
		Point_ConstIterator rightLast = pointList_.end();

		const integer rightPoints = result.second.second;
		if (rightPoints > 0)
		{
			rightFirst = result.second.first;
			rightLast = nodeEnd;
			--rightLast;
		}

		// Allocate the new leaf nodes.

		Node* left = allocateLeaf(
			node,
			leftFirst, 
			leftLast, 
			leftPoints);

		Node* right = allocateLeaf(
			node, 
			rightFirst, 
			rightLast, 
			rightPoints);

		// Turn the subdivided node into a split node.

		node->setLeft(left);
		node->setRight(right);
		node->setSplitPosition(splitPosition);
		node->setSplitAxis(splitAxis);

		// Create new cuts for the child nodes.

		Real leftMin;
		Real leftMax;
		if (simulateKdTree_)
		{
			leftMin = prevMin;
			leftMax = splitPosition;
		}
		else
		{
			const std::pair<Real, Real> leftBound = 
				computeBound(left->first(), left->end(), splitAxis);
			leftMin = leftBound.first;
			leftMax = leftBound.second;
		}

		left->setMin(leftMin);
		left->setMax(leftMax);
		left->setPrevMin(prevMin);
		left->setPrevMax(prevMax);

		Real rightMin;
		Real rightMax;
		if (simulateKdTree_)
		{
			rightMin = splitPosition;
			rightMax = prevMax;
		}
		else
		{
			const std::pair<Real, Real> rightBound = 
				computeBound(right->first(), right->end(), splitAxis);
			rightMin = rightBound.first;
			rightMax = rightBound.second;
		}

		right->setMin(rightMin);
		right->setMax(rightMax);
		right->setPrevMin(prevMin);
		right->setPrevMax(prevMax);

		// One leaf node got splitted into two,
		// so it's only one up.

		++leaves_;

		// Note: we didn't update hierarchical information
		// nor set the leaf node pointers for points.
		// This is intentional: these things are done in refine().
		// Doing them here would affect the complexity of
		// recursive subdivision.
	}

	template <typename Real, int N, typename PointPolicy>
	template <typename InputIterator>
	typename PointKdTree<Real, N, PointPolicy>::Point_Iterator
	PointKdTree<Real, N, PointPolicy>::insertPrepare(
		const InputIterator& begin, 
		const InputIterator& end)
	{
		// Copy the points to the end of pointList_.

		Point_Iterator oldLast = pointList_.end();
		if (!pointList_.empty())
		{
			--oldLast;
		}

		// We can't use the return value of std::copy because
		// it is of type std::back_inserter_iterator.
		std::copy(begin, end, std::back_inserter(pointList_));

		Point_Iterator first;
		if (oldLast == pointList_.end())
		{
			first = pointList_.begin();
		}
		else
		{
			first = oldLast;
			++first;
		}

		return first;
	}

	template <typename Real, int N, typename PointPolicy>
	void PointKdTree<Real, N, PointPolicy>::insert(
		Node* node,
		const Point_Iterator& first, 
		const Point_Iterator& last,
		integer points,
		AlignedBox<Real, N>& bound)
	{
		ASSERT(node);
		ASSERT_OP(points, >, 0);

		const Point_Iterator begin = first;
		Point_Iterator end = last;
		++end;

		if (node->leaf())
		{
			// Compute the bounding box of the inserted points.
			bound = computeBound(begin, end);

			// Set the bounds.
			updateBounds(node, bound);

			// Set the leaf nodes for each point.
			setLeaf(begin, end, node);

			// Splice the points to this node.
			pointList_.splice(node->end(), pointList_, begin, end);

			// Update the node's point range.
			node->insert(first, last, points, pointList_.end());
		}
		else
		{
			// This is a split node, so carry out a 
			// partitioning of the points.

			const SplitPredicate splitPredicate(
				node->splitPosition(), node->splitAxis(), 
				pointPolicy_);

			const std::pair<
				std::pair<Point_Iterator, integer>,
				std::pair<Point_Iterator, integer> > result =
				partition(pointList_, begin, end,
				splitPredicate);

			const Point_Iterator newRightFirst = result.second.first;
			const integer newRightPoints = result.second.second;
			Point_Iterator newRightLast = end;
			--newRightLast;

			const Point_Iterator newLeftFirst = result.first.first;
			const integer newLeftPoints = result.first.second;
			Point_Iterator newLeftLast = newRightFirst;
			--newLeftLast;

			Node* left = node->left();
			Node* right = node->right();

			if (newLeftPoints > 0)
			{
				// If there are points going to the left node,
				// recurse deeper.

				insert(
					left, 
					newLeftFirst, newLeftLast, 
					newLeftPoints,
					bound);
			}

			if (newRightPoints > 0)
			{
				// If there are points going to the right node,
				// recurse deeper.

				AlignedBox<Real, N> rightBound(
					ofDimension(dimension()));
				insert(
					right, 
					newRightFirst, newRightLast, 
					newRightPoints,
					rightBound);

				// Compute the combined bounding box of the
				// inserted points in both the left and right child.
				extendToCover(rightBound, bound);
			}

			// Update hierarchical information.
			node->updateHierarchical();
			updateBounds(node, bound);
			
			// Finally, we need to order the points in the pointList_
			// so that the points in the right child come right after
			// those in the left child. This is needed to represent
			// the point sets in each node by a simple iterator range.
			// We take care of this ordering by splicing.
			pointList_.splice(
				left->end(),
				pointList_,
				right->first(),
				right->end());
		}
	}

	template <typename Real, int N, typename PointPolicy>
	template <typename SplitRule_PointKdTree>
	void PointKdTree<Real, N, PointPolicy>::refine(
		Node* node,
		Vector<Real, N>& minBound,
		Vector<Real, N>& maxBound,
		const SplitRule_PointKdTree& splitRule,
		integer depth,
		integer bucketSize)
	{
		ASSERT(node);

		if (node->leaf())
		{
			if (node->points() <= bucketSize)
			{
				setLeaf(node->first(), node->end(), node);
			}
			else
			{
				const std::pair<Real, integer> split = 
					splitRule(*this, Cursor(node), 
					minBound, maxBound, depth);
				const Real splitPosition = split.first;
				const integer splitAxis = split.second;

				subdivide(node, splitPosition, splitAxis,
					minBound[splitAxis], maxBound[splitAxis]);
			}
		}

		if (!node->leaf())
		{
			// Refine recursively.

			const integer splitAxis = node->splitAxis();
			
			const Real oldMinBound = minBound[splitAxis];
			const Real oldMaxBound = maxBound[splitAxis];

			minBound[splitAxis] = node->left()->min();
			maxBound[splitAxis] = node->left()->max();

			refine(
				node->left(), 
				minBound,
				maxBound,
				splitRule,
				depth + 1,
				bucketSize);

			minBound[splitAxis] = node->right()->min();
			maxBound[splitAxis] = node->right()->max();

			refine(
				node->right(), 
				minBound,
				maxBound,
				splitRule,
				depth + 1,
				bucketSize);

			minBound[splitAxis] = oldMinBound;
			maxBound[splitAxis] = oldMaxBound;

			// Update point information.
			// The bound information has already
			// been handled top-down.
			node->updateHierarchical();
		}
	}

}

#endif
