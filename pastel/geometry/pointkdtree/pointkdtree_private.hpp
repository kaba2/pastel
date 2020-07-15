#ifndef PASTELGEOMETRY_POINTKDTREE_PRIVATE_HPP
#define PASTELGEOMETRY_POINTKDTREE_PRIVATE_HPP

#include "pastel/geometry/pointkdtree/pointkdtree.h"
#include "pastel/geometry/splitrule/longestmedian_splitrule.h"

#include "pastel/sys/range.h"
#include "pastel/sys/set/range_set.h"
#include "pastel/sys/set/transformed_set.h"

#include <type_traits>

namespace Pastel
{

	// Private

	template <typename Settings, template <typename> class Customization>
	void PointKdTree<Settings, Customization>::initialize()
	{
		root_ = allocateLeaf(
			0,
			pointSet_.end(), pointSet_.end(),
			0);

		++leaves_;
	}

	template <typename Settings, template <typename> class Customization>
	void PointKdTree<Settings, Customization>::copyConstruct(
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
			thisNode->setPrevMin(
				thatNode->prevMin());
			thisNode->setPrevMax(
				thatNode->prevMax());

			updateHierarchical(thisNode);
		}
	}

	template <typename Settings, template <typename> class Customization>
	typename PointKdTree<Settings, Customization>::Node*
		PointKdTree<Settings, Customization>::allocateLeaf(
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

	template <typename Settings, template <typename> class Customization>
	void PointKdTree<Settings, Customization>::destructSubtree(
		Node* node)
	{
		if (!std::is_trivially_destructible<Real>::value)
		{
			if (!node->leaf())
			{
				destructSubtree(node->left());
				destructSubtree(node->right());
			}

			destruct(node);
		}
	}

	template <typename Settings, template <typename> class Customization>
	auto PointKdTree<Settings, Customization>::computeBound(
		const Point_ConstIterator& begin, 
		const Point_ConstIterator& end) const
		-> AlignedBox<Real, N>
	{
		auto pointSet = 
			transformedSet(rangeSet(begin, end),
			[&](const PointInfo& point)
		{
			return point.point();
		});

		return boundingAlignedBox(
			locationSet(pointSet, locator()));
	}

	template <typename Settings, template <typename> class Customization>
	auto PointKdTree<Settings, Customization>::computeBound(
		const Point_ConstIterator& begin, 
		const Point_ConstIterator& end,
		integer axis) const
		-> std::pair<Real, Real>
	{
		std::pair<Real, Real> bound(
			(Real)Infinity(), -(Real)Infinity());

		Point_ConstIterator iter = begin;
		Point_ConstIterator iterEnd = end;
		while(iter != iterEnd)
		{
			Real position = 
				locator()(iter->point(), axis);
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

	template <typename Settings, template <typename> class Customization>
	void PointKdTree<Settings, Customization>::merge(
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

	template <typename Settings, template <typename> class Customization>
	void PointKdTree<Settings, Customization>::clear(
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
		if (!std::is_trivially_destructible<Real>::value)
		{
			destruct(node);
		}

		// Deallocate the memory of the node.
		nodeAllocator_.deallocate(node);
	}

	template <typename Settings, template <typename> class Customization>
	void PointKdTree<Settings, Customization>::setLeaf(
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

	template <typename Settings, template <typename> class Customization>
	void PointKdTree<Settings, Customization>::updateBounds(
		Node* node, const AlignedBox<Real, N>& bound)
	{
		// We only allow to expand the
		// point bound.
		if (node->parent())
		{
			integer splitAxis = node->parent()->splitAxis();
			node->setMin(std::min(bound.min()[splitAxis], node->min()));
			node->setMax(std::max(bound.max()[splitAxis], node->max()));
		}
	}

	template <typename Settings, template <typename> class Customization>
	void PointKdTree<Settings, Customization>::updateHierarchical(
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

		Point_ConstIterator first = 
			left->empty() ? right->first() : left->first();

		Point_ConstIterator last = 
			right->empty() ? left->last() : right->last();

		node->setFirst(first);
		node->setLast(last);
		node->setPoints(
			left->points() + right->points());
	}

	template <typename Settings, template <typename> class Customization>
	void PointKdTree<Settings, Customization>::erase(
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

		// Actually remove the points.
		pointSet_.erase(node->first(), node->end());

		// Clear the point ranges in the subtree.
		clearPoints(node);

		// Update the path to the root.
		updateUpwards(node);
	}

	template <typename Settings, template <typename> class Customization>
	void PointKdTree<Settings, Customization>::clearPoints(
		Node* node)
	{
		ASSERT(node);

		// Update the node state.

		node->setFirst(pointSet_.end());
		node->setLast(pointSet_.end());
		node->setPoints(0);

		if (!node->leaf())
		{
			// Recurse deeper.

			clearPoints(node->right());
			clearPoints(node->left());
		}
	}

	template <typename Settings, template <typename> class Customization>
	void PointKdTree<Settings, Customization>::subdivide(
		Node* node,
		const Real& splitPosition, 
		integer splitAxis,
		const Real& prevMin,
		const Real& prevMax)
	{
		ASSERT(node);
		ASSERT(node->leaf());
		ASSERT2(splitAxis >= 0 && splitAxis < n(), splitAxis, n());

		Point_ConstIterator nodeEnd = node->end();

		// Reorder the points along the split position.

		SplitPredicate splitPredicate(
			splitPosition, splitAxis, 
			locator());

		std::pair<std::pair<Point_Iterator, integer>,
			std::pair<Point_Iterator, integer> > result =
			partition(pointSet_, Pastel::range(node->first(), nodeEnd),
			splitPredicate);

		Point_ConstIterator leftFirst = end();
		Point_ConstIterator leftLast = end();

		integer leftPoints = result.first.second;
		if (leftPoints > 0)
		{
			leftFirst = result.first.first;
			leftLast = result.second.first;
			--leftLast;
		}

		Point_ConstIterator rightFirst = end();
		Point_ConstIterator rightLast = end();

		integer rightPoints = result.second.second;
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
			std::pair<Real, Real> leftBound = 
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
			std::pair<Real, Real> rightBound = 
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

	template <typename Settings, template <typename> class Customization>
	template <Range_Concept PointId_Range>
	auto PointKdTree<Settings, Customization>::copyToEnd(
		const PointId_Range& pointSet, 
		bool hidden)
		-> Point_Iterator
	{
		ASSERT(!isEmptyRange(pointSet));

		auto&& index = ranges::begin(pointSet);

		{
			pointSet_.insertBack(
				PointInfo(
					*index, 
					0, 
					hidden
				)
			);
			++index;			
		}

		Point_Iterator first = pointSet_.end();
		--first;

		while(index != ranges::end(pointSet))
		{
			pointSet_.insertBack(
				PointInfo(
					*index, 
					0, 
					hidden
				)
			);

			++index;
		}
	
		return first;
	}

	template <typename Settings, template <typename> class Customization>
	void PointKdTree<Settings, Customization>::insert(
		Node* node,
		const Point_Iterator& first, 
		const Point_Iterator& last,
		integer points,
		AlignedBox<Real, N>& bound)
	{
		ASSERT(node);
		ASSERT_OP(points, >, 0);
		ASSERT(first != pointSet_.end());

		Point_Iterator begin = first;
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

			SplitPredicate splitPredicate(
				node->splitPosition(), node->splitAxis(), 
				locator());

			std::pair<
				std::pair<Point_Iterator, integer>,
				std::pair<Point_Iterator, integer> > result =
				partition(pointSet_, Pastel::range(begin, end),
				splitPredicate);

			Point_Iterator newRightFirst = result.second.first;
			integer newRightPoints = result.second.second;
			Point_Iterator newRightLast = end;
			--newRightLast;

			Point_Iterator newLeftFirst = result.first.first;
			integer newLeftPoints = result.first.second;
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
					ofDimension(n()));
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

	template <typename Settings, template <typename> class Customization>
	template <typename SplitRule>
	void PointKdTree<Settings, Customization>::refine(
		Node* node,
		AlignedBox<Real, N>& bound,
		const SplitRule& splitRule,
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
				auto pointSet = 
					transformedSet(
						rangeSet(node->first(), node->end()),
						[&](const PointInfo& point)
						{
							return point.point();
						});

				std::pair<Real, integer> split = 
					splitRule(
						locationSet(pointSet, locator()), 
						bound
					);

				Real splitPosition = split.first;
				integer splitAxis = split.second;

				subdivide(
					node, splitPosition, splitAxis,
					bound.min()[splitAxis], 
					bound.max()[splitAxis]);
			}
		}

		if (!node->leaf())
		{
			// Refine recursively.

			integer splitAxis = node->splitAxis();
			
			Real oldMinBound = bound.min()[splitAxis];
			Real oldMaxBound = bound.max()[splitAxis];

			bound.min()[splitAxis] = node->left()->min();
			bound.max()[splitAxis] = node->left()->max();

			refine(
				node->left(), 
				bound,
				splitRule,
				depth + 1,
				bucketSize);

			bound.min()[splitAxis] = node->right()->min();
			bound.max()[splitAxis] = node->right()->max();

			refine(
				node->right(), 
				bound,
				splitRule,
				depth + 1,
				bucketSize);

			bound.min()[splitAxis] = oldMinBound;
			bound.max()[splitAxis] = oldMaxBound;

			// Update point information.
			// The bound information has already
			// been handled top-down.
			updateHierarchical(node);
		}
	}

	template <typename Settings, template <typename> class Customization>
	void PointKdTree<Settings, Customization>::commitInsertion()
	{
		if (insertionSet_.empty())
		{
			return;
		}

		// Count the number of inserted points.
		integer points = insertionSet_.size();

		// Find out the points to insert.
		Point_Iterator first = insertionSet_.begin();
		Point_Iterator last = insertionSet_.end();
		--last;

		// Move the points to the end of 'pointSet_'.
		pointSet_.splice(pointSet_.end(), insertionSet_);

		// Send the points down the tree.
		AlignedBox<Real, N> pointBound(
			ofDimension(n()));
		insert(root_, first, last, points, pointBound);

		// Update the bounding box.
		extendToCover(pointBound, bound_);
	}

	template <typename Settings, template <typename> class Customization>
	void PointKdTree<Settings, Customization>::commitErase(
		const Point_ConstIterator& iter)
	{
		ASSERT(iter != pointSet_.end());

		if (iter->leaf() && 
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

	template <typename Settings, template <typename> class Customization>
	void PointKdTree<Settings, Customization>::commitHide(
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

		if (iter->leaf())
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
				iter);

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
				iter);
		}

		iter->setHidden(true);
	}

	template <typename Settings, template <typename> class Customization>
	void PointKdTree<Settings, Customization>::commitShow(
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

		if (iter->leaf())
		{
			// The point has an associated node.
			Node* node = iter->leaf().node_;

			ASSERT(node->leaf());

			// Move the point to the end of 'pointSet'.
			pointSet_.splice(
				pointSet_.end(),
				hiddenSet_,
				iter);

			// Insert the point back.
			AlignedBox<Real, N> bound(
				ofDimension(n()));
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
				iter);

			// With immediate updates, we insert the
			// point straight away.
			commitInsertion();
		}
	}

	template <typename Settings, template <typename> class Customization>
	void PointKdTree<Settings, Customization>::updateDownwards(
		Node* node)
	{
		if (node->invalid())
		{
			updateDownwards(node->left());
			updateDownwards(node->right());

			updateHierarchical(node);
		}
	}

	template <typename Settings, template <typename> class Customization>
	void PointKdTree<Settings, Customization>::updateUpwards(
		Node* node)
	{
		ASSERT(node);

		node = node->parent();
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

	template <typename Settings, template <typename> class Customization>
	void PointKdTree<Settings, Customization>::setHidden(
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
