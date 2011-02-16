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
			pointSet_.end(), pointSet_.end(),
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
				thatNode->splitAxis(),
				thatNode->prevMin(),
				thatNode->prevMax());

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
			updateHierarchical(thisNode);
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
			parent,	0, 0,
			first, last,
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
				pointPolicy_.axis(iter->point(), axis);
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

		// This function destructs the subdivision
		// in the subtree under 'node'. The 'node'
		// is then made a leaf node, with its points
		// given by those in the old subtree.

		if (node->leaf())
		{
			// This is already a leaf node.
			// Nothing to do.
			return;
		}
		
		if (node == root_)
		{
			// In case the whole tree is merged into
			// a single node, this can be done faster
			// by the 'merge()' function.
			merge();
		}
		else
		{
			// Destruct the subdivision structure
			// below.
			clear(node->left());
			clear(node->right());

			// Turning a node into a leaf node
			// is as simple as zeroing the
			// children.
			node->setLeft(0);
			node->setRight(0);

			// Set the node pointers to this node.
			setLeaf(node->first(), node->end(), node);

			// Some of the hidden points might have had
			// destructed nodes as their associated nodes.
			// Therefore, we remove all associations from the
			// hidden points, forcing them to be inserted
			// from the root.
			setLeaf(hiddenSet_.begin(), hiddenSet_.end(), 0);
		}
	}

	template <typename Real, int N, typename PointPolicy>
	void PointKdTree<Real, N, PointPolicy>::clear(
		Node* node)
	{
		ASSERT(node);

		// This function destructs the subdivision
		// structure in the subtree of 'node', but 
		// does not remove points.

		if (node->leaf())
		{
			// This is a leaf node. Update the number 
			// of leaf nodes.
			--leaves_;
		}
		else
		{
			// This is a split node.
			// Destruct the child subtrees.
			clear(node->left());
			clear(node->right());
		}

		// Destruct the node.
		if (!boost::has_trivial_destructor<Real>())
		{
			StdExt::destruct(node);
		}

		// Deallocate the memory of the node.
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
	void PointKdTree<Real, N, PointPolicy>::updateHierarchical(
		Node* node)
	{
		ASSERT(node);
		ASSERT(!node->leaf());

		Node* left = node->left();
		Node* right = node->right();

		ASSERT(!left->invalid());
		ASSERT(!right->invalid());

		// As an invariant, we maintain the points in the pointSet_
		// so that the points in the right child come right after
		// those in the left child. This is needed to represent
		// the point sets in each node by a simple iterator range.
		// We take care of this ordering by splicing.
		pointSet_.splice(
			left->end(), pointSet_,
			right->first(),	right->end());

		const Point_ConstIterator first = 
			left->empty() ? right->first() : left->first();

		const Point_ConstIterator last = 
			right->empty() ? left->last() : right->last();

		node->setFirst(first);
		node->setLast(last);
		node->setPoints(
			left->points() + right->points());
	}

	template <typename Real, int N, typename PointPolicy>
	void PointKdTree<Real, N, PointPolicy>::erase(
		Node* node)
	{
		ASSERT(node);

		if (node == root_)
		{
			// Make use of the optimization for the 
			// whole tree.
			erase();
			return;
		}

		// This operation is always immediate.
		if (lazyUpdates_)
		{
			update();
		}

		// Actually remove the points.
		pointSet_.erase(node->first(), node->end());

		// Clear the point ranges in the subtree.
		clearPoints(node);

		// Update the path to the root.
		updateUpwards(node);
	}

	template <typename Real, int N, typename PointPolicy>
	void PointKdTree<Real, N, PointPolicy>::clearPoints(
		Node* node)
	{
		ASSERT(node);

		// Update the node state.

		node->setFirst(pointSet_.end());
		node->setLast(pointSet_.end());
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

		const Point_ConstIterator nodeEnd = node->end();

		// Reorder the points along the split position.

		const SplitPredicate splitPredicate(
			splitPosition, splitAxis, 
			pointPolicy_);

		const std::pair<std::pair<Point_Iterator, integer>,
			std::pair<Point_Iterator, integer> > result =
			partition(pointSet_, node->first(), nodeEnd,
			splitPredicate);

		Point_ConstIterator leftFirst = end();
		Point_ConstIterator leftLast = end();

		const integer leftPoints = result.first.second;
		if (leftPoints > 0)
		{
			leftFirst = result.first.first;
			leftLast = result.second.first;
			--leftLast;
		}

		Point_ConstIterator rightFirst = end();
		Point_ConstIterator rightLast = end();

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
	template <typename Input_Point_ConstIterator>
	typename PointKdTree<Real, N, PointPolicy>::Point_Iterator
	PointKdTree<Real, N, PointPolicy>::copyToEnd(
		const Input_Point_ConstIterator& begin, 
		const Input_Point_ConstIterator& end,
		bool hidden)
	{
		ASSERT(begin != end);

		Input_Point_ConstIterator iter = begin;

		pointSet_.push_back(
			PointInfo(*iter, 0, hidden));
		++iter;

		Point_Iterator first = pointSet_.end();
		--first;

		while(iter != end)
		{
			pointSet_.push_back(
				PointInfo(*iter, 0, hidden));

			++iter;
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
		ASSERT(first != pointSet_.end());

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
			pointSet_.splice(node->end(), pointSet_, begin, end);

			// Update the node's point range.
			node->insert(first, last, points, pointSet_.end());
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
				partition(pointSet_, begin, end,
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
			else if (left->invalid())
			{
				updateDownwards(left);
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
			else if (right->invalid())
			{
				updateDownwards(right);
			}

			// Update hierarchical information.
			updateHierarchical(node);
			updateBounds(node, bound);
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
			updateHierarchical(node);
		}
	}

	template <typename Real, int N, typename PointPolicy>
	void PointKdTree<Real, N, PointPolicy>::commitInsertion()
	{
		if (insertionSet_.empty())
		{
			return;
		}

		// Count the number of inserted points.
		const integer points = insertionSet_.size();

		// Find out the points to insert.
		Point_Iterator first = insertionSet_.begin();
		Point_Iterator last = insertionSet_.end();
		--last;

		// Move the points to the end of 'pointSet_'.
		pointSet_.splice(pointSet_.end(), insertionSet_);

		// Send the points down the tree.
		AlignedBox<Real, N> pointBound(
			ofDimension(dimension()));
		insert(root_, first, last, points, pointBound);

		// Update the bounding box.
		extendToCover(pointBound, bound_);
	}

	template <typename Real, int N, typename PointPolicy>
	void PointKdTree<Real, N, PointPolicy>::commitErase(
		const Point_ConstIterator& iter)
	{
		ASSERT(iter != pointSet_.end());

		if (iter->leaf().exists() && 
			!iter->hidden())
		{
			// The point is in a node.
			Node* node = iter->leaf().node_;

			// Remove reference to the point
			// from this node.
			node->erase(iter, pointSet_.end());

			// Actually remove the point from the point list.
			pointSet_.erase(iter);

			// Update the path to the root.
			updateUpwards(node);
		}
		else
		{
			// If the point does not have an associated
			// node, or is hidden, simply remove it.
			if (iter->hidden())
			{
				hiddenSet_.erase(iter);
			}
			else
			{
				insertionSet_.erase(iter);
			}
		}
	}

	template <typename Real, int N, typename PointPolicy>
	void PointKdTree<Real, N, PointPolicy>::commitHide(
		const Point_ConstIterator& point)
	{
		ASSERT(point != pointSet_.end());

		if (point->hidden())
		{
			// The point is already hidden.
			// Nothing to do.
			return;
		}

		Point_Iterator iter = pointSet_.cast(point);
		Point_Iterator nextIter = iter;
		++nextIter;

		if (iter->leaf().exists())
		{
			// The point is in a node.
			Node* node = iter->leaf().node_;

			// Remove reference to the point
			// from this node.
			node->erase(iter, pointSet_.end());

			// Move the point to the hidden region.
			hiddenSet_.splice(
				hiddenSet_.end(),
				pointSet_,
				iter, nextIter);

			// Update the path to the root.
			updateUpwards(node);
		}
		else
		{
			// The point is waiting for insertion.

			// Move the point to the hidden region.
			hiddenSet_.splice(
				hiddenSet_.end(),
				pointSet_,
				iter, nextIter);
		}

		iter->setHidden(true);
	}

	template <typename Real, int N, typename PointPolicy>
	void PointKdTree<Real, N, PointPolicy>::commitShow(
		const Point_ConstIterator& point)
	{
		ASSERT(point != pointSet_.end());

		if (!point->hidden())
		{
			// The point is already shown.
			// Nothing to do.
			return;
		}

		Point_Iterator iter = hiddenSet_.cast(point);
		Point_Iterator nextIter = iter;
		++nextIter;

		iter->setHidden(false);

		if (iter->leaf().exists())
		{
			// The point has an associated node.
			Node* node = iter->leaf().node_;

			ASSERT(node->leaf());

			// Move the point to the end of 'pointSet'.
			pointSet_.splice(
				pointSet_.end(),
				hiddenSet_,
				iter, nextIter);

			// Insert the point back.
			AlignedBox<Real, N> bound(
				ofDimension(dimension()));
			insert(node, iter, iter, 1, bound);

			// Update the path to the root.
			updateUpwards(node);
		}
		else
		{
			// Move the point to the 'insertionSet_'.
			insertionSet_.splice(
				insertionSet_.end(),
				hiddenSet_,
				iter, nextIter);

			if (!lazyUpdates_)
			{
				// With immediate updates, we insert the
				// point straight away.
				commitInsertion();
			}
		}
	}

	template <typename Real, int N, typename PointPolicy>
	void PointKdTree<Real, N, PointPolicy>::updateDownwards(
		Node* node)
	{
		if (node->invalid())
		{
			updateDownwards(node->left());
			updateDownwards(node->right());

			updateHierarchical(node);
		}
	}

	template <typename Real, int N, typename PointPolicy>
	void PointKdTree<Real, N, PointPolicy>::updateUpwards(
		Node* node)
	{
		ASSERT(node);

		node = node->parent();
		if (lazyUpdates_)
		{
			// With lazy updates we mark the
			// nodes on the path to the root invalid.
			while (node && !node->invalid())
			{
				node->invalidate();
				node = node->parent();
			}
		}
		else
		{
			// With immediate updates we update the
			// hierarchical information straight away.
			while(node)
			{
				ASSERT(!node->left()->invalid());
				ASSERT(!node->right()->invalid());

				updateHierarchical(node);

				node = node->parent();
			}
		}
	}

	template <typename Real, int N, typename PointPolicy>
	void PointKdTree<Real, N, PointPolicy>::setHidden(
		const Point_ConstIterator& begin, 
		const Point_ConstIterator& end,
		bool hidden)
	{
		Point_ConstIterator iter = begin;
		while(iter != end)
		{
			iter->setHidden(hidden);
			++iter;
		}
	}

}

#endif
