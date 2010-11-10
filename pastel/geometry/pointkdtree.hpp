#ifndef PASTEL_POINTKDTREE_HPP
#define PASTEL_POINTKDTREE_HPP

#include "pastel/geometry/pointkdtree.h"
#include "pastel/geometry/bounding_alignedbox.h"

#include "pastel/sys/ensure.h"
#include "pastel/sys/fastlist_tools.h"

#include <boost/operators.hpp>

namespace Pastel
{

	template <typename Real, int N, typename PointPolicy>
	PointKdTree<Real, N, PointPolicy>::PointKdTree(
		bool simulateKdTree,
		const PointPolicy& pointPolicy)
		: pointList_()
		, nodeAllocator_(sizeof(Node))
		, root_(0)
		, leaves_(0)
		, pointPolicy_(pointPolicy)
		, bound_(pointPolicy.dimension())
		, simulateKdTree_(simulateKdTree)
	{
		ENSURE(N == Dynamic || 
			N == pointPolicy.dimension());

		pointList_.set_allocator(PointContainer::allocator_ptr(
			new PointAllocator(pointList_.get_allocator()->unitSize())));

		initialize();
	}

	template <typename Real, int N, typename PointPolicy>
	PointKdTree<Real, N, PointPolicy>::PointKdTree(const PointKdTree& that)
		: pointList_()
		, nodeAllocator_(sizeof(Node))
		, root_(0)
		, leaves_(0)
		, pointPolicy_(that.pointPolicy_)
		, bound_(that.bound_)
		, simulateKdTree_(that.simulateKdTree)
	{
		pointList_.set_allocator(PointContainer::allocator_ptr(
			new PointAllocator(pointList_.get_allocator()->unitSize())));

		initialize();

		// First copy the structure of the tree.

		copyConstruct(root_, that.root_);

		// Then insert the points to the nodes.

		insert(
			forwardRange(that.pointBegin(), that.pointEnd()));
	}

	template <typename Real, int N, typename PointPolicy>
	PointKdTree<Real, N, PointPolicy>::~PointKdTree()
	{
		PASTEL_STATIC_ASSERT(N > 0 || N == Dynamic);

		/*
		Time complexity:
		
		* Freeing the raw memory for the m nodes takes O(m), 
		although we do not necessarily need to run their destructors.

		* Destructing the point list with n points 
		all at once takes O(n).
		*/

		destructSubtree(root_);
		nodeAllocator_.clear();
	}

	template <typename Real, int N, typename PointPolicy>
	PointKdTree<Real, N, PointPolicy>&
		PointKdTree<Real, N, PointPolicy>::operator=(
		const PointKdTree& that)
	{
		PointKdTree copy(that);
		swap(copy);
		return *this;
	}

	template <typename Real, int N, typename PointPolicy>
	void PointKdTree<Real, N, PointPolicy>::swap(
		PointKdTree& that)
	{
		pointList_.swap(that.pointList_);
		nodeAllocator_.swap(that.nodeAllocator_);
		std::swap(root_, that.root_);
		std::swap(leaves_, that.leaves_);
		std::swap(pointPolicy_, that.pointPolicy_);
		bound_.swap(that.bound_);
		std::swap(simulateKdTree_, that.simulateKdTree_);
	}

	template <typename Real, int N, typename PointPolicy>
	const PointPolicy& PointKdTree<Real, N, PointPolicy>::pointPolicy() const
	{
		return pointPolicy_;
	}

	template <typename Real, int N, typename PointPolicy>
	const AlignedBox<Real, N>& 
		PointKdTree<Real, N, PointPolicy>::bound() const
	{
		return bound_;
	}

	template <typename Real, int N, typename PointPolicy>
	void PointKdTree<Real, N, PointPolicy>::reserveBound(
		const AlignedBox<Real, N>& boxToCover)
	{
		extendToCover(boxToCover, bound_);
	}

	template <typename Real, int N, typename PointPolicy>
	bool PointKdTree<Real, N, PointPolicy>::empty() const
	{
		return pointList_.empty();
	}

	template <typename Real, int N, typename PointPolicy>
	typename PointKdTree<Real, N, PointPolicy>::Cursor
		PointKdTree<Real, N, PointPolicy>::root() const
	{
		return Cursor(root_);
	}

	template <typename Real, int N, typename PointPolicy>
	typename PointKdTree<Real, N, PointPolicy>::ConstPointIterator
		PointKdTree<Real, N, PointPolicy>::begin() const
	{
		return pointList_.begin();
	}

	template <typename Real, int N, typename PointPolicy>
	typename PointKdTree<Real, N, PointPolicy>::ConstPointDataIterator
		PointKdTree<Real, N, PointPolicy>::pointBegin() const
	{
		return ConstPointDataIterator(pointList_.begin());
	}

	template <typename Real, int N, typename PointPolicy>
	typename PointKdTree<Real, N, PointPolicy>::ConstPointIterator
		PointKdTree<Real, N, PointPolicy>::end() const
	{
		return pointList_.end();
	}

	template <typename Real, int N, typename PointPolicy>
	typename PointKdTree<Real, N, PointPolicy>::ConstPointDataIterator
		PointKdTree<Real, N, PointPolicy>::pointEnd() const
	{
		return ConstPointDataIterator(pointList_.end());
	}

	template <typename Real, int N, typename PointPolicy>
	integer PointKdTree<Real, N, PointPolicy>::nodes() const
	{
		return nodeAllocator_.allocated();
	}

	template <typename Real, int N, typename PointPolicy>
	integer PointKdTree<Real, N, PointPolicy>::leaves() const
	{
		return leaves_;
	}

	template <typename Real, int N, typename PointPolicy>
	integer PointKdTree<Real, N, PointPolicy>::points() const
	{
		return pointList_.size();
	}

	template <typename Real, int N, typename PointPolicy>
	integer PointKdTree<Real, N, PointPolicy>::dimension() const
	{
		return pointPolicy_.dimension();
	}
	
	template <typename Real, int N, typename PointPolicy>
	template <typename SplitRule_PointKdTree>
	void PointKdTree<Real, N, PointPolicy>::refine(
		const SplitRule_PointKdTree& splitRule,
		integer bucketSize)
	{
		Vector<Real, N> minBound(bound_.min());
		Vector<Real, N> maxBound(bound_.max());

		refine(root_, 
			minBound,
			maxBound,
			splitRule,
			0,
			bucketSize);
	}

	template <typename Real, int N, typename PointPolicy>
	typename PointKdTree<Real, N, PointPolicy>::ConstPointIterator 
		PointKdTree<Real, N, PointPolicy>::insert(const Point& point)
	{
		// Copy the point to the end of pointList_.

		pointList_.push_back(point);
		
		PointIterator iter = pointList_.end();
		--iter;

		// Splice the point to the leaf node.

		AlignedBox<Real, N> pointBound(
			ofDimension(dimension()));

		insert(root_, iter, iter, 1, pointBound);

		extendToCover(pointBound, bound_);

		return iter;
	}

	template <typename Real, int N, typename PointPolicy>
	template <typename InputIterator,
		typename ConstPointIterator_OutputIterator>
	void PointKdTree<Real, N, PointPolicy>::insert(
		const ForwardRange<InputIterator>& pointSet, 
		ConstPointIterator_OutputIterator iteratorSet)
	{
		if (pointSet.empty())
		{
			// Nothing to do.
			return;
		}

		const InputIterator begin = pointSet.begin();
		const InputIterator end = pointSet.end();

		// Prepare for insertion.
		const PointIterator first = insertPrepare(begin, end);

		// Copy the new point iterators to the user.
		std::copy(countingIterator(first),
			countingIterator(pointList_.end()),
			iteratorSet);

		// Send the points down the tree.
		const integer points = std::distance(begin, end);
		PointIterator last = pointList_.end();
		--last;

		AlignedBox<Real, N> pointBound(
			ofDimension(dimension()));
		insert(root_, first, last, points, pointBound);

		extendToCover(pointBound, bound_);
	}

	template <typename Real, int N, typename PointPolicy>
	template <typename InputIterator>
	void PointKdTree<Real, N, PointPolicy>::insert(
		const ForwardRange<InputIterator>& pointSet)
	{
		if (pointSet.empty())
		{
			// Nothing to do.
			return;
		}

		const InputIterator begin = pointSet.begin();
		const InputIterator end = pointSet.end();

		// Prepare for insertion.
		const PointIterator first = insertPrepare(begin, end);

		// Send the points down the tree.
		const integer points = std::distance(begin, end);

		PointIterator last = pointList_.end();
		--last;

		AlignedBox<Real, N> pointBound(
			ofDimension(dimension()));
		insert(root_, first, last, points, pointBound);

		extendToCover(pointBound, bound_);
	}

	template <typename Real, int N, typename PointPolicy>
	void PointKdTree<Real, N, PointPolicy>::erase(
		const ConstPointIterator& iter)
	{
		Node* node = iter->leaf().node_;

		// Remove reference to the point
		// from this node.
		node->erase(iter, pointList_.end());

		// Actually remove the point from the point list.
		pointList_.erase(iter);

		// Propagate point set changes upwards.
		updatePointsUpwards(node);
	}

	template <typename Real, int N, typename PointPolicy>
	template <typename ConstPointIterator_ConstIterator>
	void PointKdTree<Real, N, PointPolicy>::erase(
		const ForwardRange<ConstPointIterator_ConstIterator>& pointSet)
	{
		const ConstPointIterator_ConstIterator iter = 
			pointSet.begin();
		const ConstPointIterator_ConstIterator iterEnd = 
			pointSet.end();
		while(iter != iterEnd)
		{
			erase(*iter);
			++iter;
		}
	}

	template <typename Real, int N, typename PointPolicy>
	void PointKdTree<Real, N, PointPolicy>::clear()
	{
		pointList_.clear();
		destructSubtree(root_);
		nodeAllocator_.clear();
		root_ = 0;
		leaves_ = 0;
		bound_ = AlignedBox<Real, N>(
			ofDimension(dimension()));

		initialize();
	}

	template <typename Real, int N, typename PointPolicy>
	void PointKdTree<Real, N, PointPolicy>::erasePoints()
	{
		// We can clear all of the points faster and
		// more storage-efficiently than using 
		// erasePoints(root()).

		// Actually remove the points.
		
		pointList_.clear();

		// Clear the point ranges in the subtree.

		clearPoints(root_);
	}

	template <typename Real, int N, typename PointPolicy>
	void PointKdTree<Real, N, PointPolicy>::erasePoints(
		const Cursor& cursor)
	{
		erasePoints(cursor.node_);
	}

	template <typename Real, int N, typename PointPolicy>
	void PointKdTree<Real, N, PointPolicy>::merge()
	{
		// We can merge all of the nodes faster and
		// more storage-efficiently than using
		// merge(root_).

		ConstPointIterator begin = root_->first();
		ConstPointIterator last = root_->last();

		destructSubtree(root_);
		nodeAllocator_.clear();

		root_ = allocateLeaf(
			0,
			begin, 
			last,
			points());

		leaves_ = 1;
	}

	template <typename Real, int N, typename PointPolicy>
	void PointKdTree<Real, N, PointPolicy>::merge(
		const Cursor& cursor)
	{
		// Note: We take the optimization for 
		// the case cursor == root() inside this
		// function.
		merge(cursor.node_);
	}

	template <typename Real, int N, typename PointPolicy>
	Vector<Real, N> PointKdTree<Real, N, PointPolicy>::point(
		const Point& point) const
	{
		return pointPolicy_(point);
	}

}

#endif
