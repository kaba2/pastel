#ifndef PASTELGEOMETRY_POINTKDTREE_HPP
#define PASTELGEOMETRY_POINTKDTREE_HPP

#include "pastel/geometry/pointkdtree.h"
#include "pastel/geometry/bounding_alignedbox.h"

#include "pastel/sys/ensure.h"
#include "pastel/sys/list.h"
#include "pastel/sys/constant_iterator.h"
#include "pastel/sys/null_output.h"

#include <boost/operators.hpp>

namespace Pastel
{

	template <typename Real, int N, typename PointPolicy>
	PointKdTree<Real, N, PointPolicy>::PointKdTree(
		const PointPolicy& pointPolicy,
		bool simulateKdTree)
		: pointSet_()
		, hiddenSet_()
		, insertionSet_()
		, nodeAllocator_(sizeof(Node))
		, root_(0)
		, leaves_(0)
		, pointPolicy_(pointPolicy)
		, bound_(pointPolicy.n())
		, simulateKdTree_(simulateKdTree)
	{
		ENSURE(N == Dynamic || 
			N == pointPolicy.n());

		initialize();
	}

	template <typename Real, int N, typename PointPolicy>
	PointKdTree<Real, N, PointPolicy>::PointKdTree(const PointKdTree& that)
		: pointSet_()
		, hiddenSet_()
		, insertionSet_()
		, nodeAllocator_(sizeof(Node))
		, root_(0)
		, leaves_(0)
		, pointPolicy_(that.pointPolicy_)
		, bound_(that.bound_)
		, simulateKdTree_(that.simulateKdTree_)
	{
		initialize();

		// First copy the structure of the tree.
		copyConstruct(root_, that.root_);

		// Then insert the points into the nodes.
		insertRange(that.asPointData(that.range()));

		// Insert the hidden points.
		insertRange(that.asPointData(that.hiddenRange()), Null_Output(), true);

		// Insert the insertion points.
		insertRange(that.asPointData(
			Pastel::range(that.insertionSet_.begin(), that.insertionSet_.end())));
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
		using std::swap;

		pointSet_.swap(that.pointSet_);
		hiddenSet_.swap(that.hiddenSet_);
		insertionSet_.swap(that.insertionSet_);
		nodeAllocator_.swap(that.nodeAllocator_);
		swap(root_, that.root_);
		swap(leaves_, that.leaves_);
		swap(pointPolicy_, that.pointPolicy_);
		bound_.swap(that.bound_);
		swap(simulateKdTree_, that.simulateKdTree_);
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
		return pointSet_.empty();
	}

	template <typename Real, int N, typename PointPolicy>
	typename PointKdTree<Real, N, PointPolicy>::Cursor
		PointKdTree<Real, N, PointPolicy>::root() const
	{
		return Cursor(root_);
	}

	template <typename Real, int N, typename PointPolicy>
	typename PointKdTree<Real, N, PointPolicy>::Point_ConstIterator
		PointKdTree<Real, N, PointPolicy>::begin() const
	{
		return pointSet_.begin();
	}

	template <typename Real, int N, typename PointPolicy>
	typename PointKdTree<Real, N, PointPolicy>::Point_ConstIterator
		PointKdTree<Real, N, PointPolicy>::end() const
	{
		return pointSet_.end();
	}

	template <typename Real, int N, typename PointPolicy>
	typename PointKdTree<Real, N, PointPolicy>::Point_ConstRange
		PointKdTree<Real, N, PointPolicy>::range() const
	{
		return Pastel::range(begin(), end());
	}

	template <typename Real, int N, typename PointPolicy>
	typename PointKdTree<Real, N, PointPolicy>::Point_ConstIterator
		PointKdTree<Real, N, PointPolicy>::hiddenBegin() const
	{
		return hiddenSet_.begin();
	}

	template <typename Real, int N, typename PointPolicy>
	typename PointKdTree<Real, N, PointPolicy>::Point_ConstIterator
		PointKdTree<Real, N, PointPolicy>::hiddenEnd() const
	{
		return hiddenSet_.end();
	}

	template <typename Real, int N, typename PointPolicy>
	typename PointKdTree<Real, N, PointPolicy>::Point_ConstRange
		PointKdTree<Real, N, PointPolicy>::hiddenRange() const
	{
		return Pastel::range(hiddenBegin(), hiddenEnd());
	}

	template <typename Real, int N, typename PointPolicy>
	typename PointKdTree<Real, N, PointPolicy>::PointData_ConstIterator
		PointKdTree<Real, N, PointPolicy>::asPointData(
			const Point_ConstIterator& iter) const
	{
		return PointData_ConstIterator(iter);
	}

	template <typename Real, int N, typename PointPolicy>
	typename PointKdTree<Real, N, PointPolicy>::PointData_ConstRange
		PointKdTree<Real, N, PointPolicy>::asPointData(
			const Point_ConstRange& range) const
	{
		return Pastel::range(
			asPointData(range.begin()), asPointData(range.end()));
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
		ASSERT_OP(root_->points(), >=, 0);

		return root_->points();
	}

	template <typename Real, int N, typename PointPolicy>
	integer PointKdTree<Real, N, PointPolicy>::n() const
	{
		return pointPolicy_.n();
	}
	
	template <typename Real, int N, typename PointPolicy>
	template <typename SplitRule>
	void PointKdTree<Real, N, PointPolicy>::refine(
		const SplitRule& splitRule,
		integer bucketSize)
	{
		Vector<Real, N> minBound(bound_.min());
		Vector<Real, N> maxBound(bound_.max());

		refine(root_, 
			minBound, maxBound,
			splitRule,
			0,
			bucketSize);

		// After refinement, a leaf node containing a
		// hidden point might be turned into a split node.
		// In this case we remove the associated node,
		// essentially turning the point into a hidden point 
		// waiting for insertion.

		Point_ConstIterator iter = hiddenSet_.begin();
		Point_ConstIterator iterEnd = hiddenSet_.end();
		while(iter != iterEnd)
		{
			Node* node = iter->leaf().node_;
			if (node && !node->leaf())
			{
				iter->setLeaf(0);
			}

			++iter;
		}
	}

	template <typename Real, int N, typename PointPolicy>
	typename PointKdTree<Real, N, PointPolicy>::Point_ConstIterator 
		PointKdTree<Real, N, PointPolicy>::insert(
		const Point& point,
		bool hidden)
	{
		Point_ConstIterator iter;

		auto report = [&](const Point_ConstIterator& iter_)
		{
			iter = iter_;
		};
		
		insertRange(constantRange(point, 1), report, hidden);

		return iter;
	}

	template <typename Real, int N, typename PointPolicy>
	template <
		typename Input_Point_ConstRange,
		typename Point_ConstIterator_Output>
	void PointKdTree<Real, N, PointPolicy>::insertRange(
		const Input_Point_ConstRange& pointSet, 
		const Point_ConstIterator_Output& report,
		bool hidden)
	{
		if (pointSet.empty())
		{
			// Nothing to do.
			return;
		}

		typedef typename boost::range_iterator<Input_Point_ConstRange>::type
			Input_Point_ConstIterator;

		const Input_Point_ConstIterator inputBegin = pointSet.begin();
		const Input_Point_ConstIterator inputEnd = pointSet.end();

		// Copy the points to the end of 'pointSet_'.
		const Point_Iterator first = 
			copyToEnd(inputBegin, inputEnd, hidden);

		// Copy the new point iterators to the user.
		for (auto point = first; point != pointSet_.end();++point)
		{
			report(point);
		}

		if (hidden)
		{
			// Move the points to the hidden region.
			hiddenSet_.splice(
				hiddenSet_.end(),
				pointSet_,
				first, pointSet_.end());
		}
		else
		{
			// Move the points to the insertion region.
			insertionSet_.splice(
				insertionSet_.end(),
				pointSet_,
				first, pointSet_.end());

			commitInsertion();
		}
	}

	template <typename Real, int N, typename PointPolicy>
	template <typename Input_Point_ConstRange>
	void PointKdTree<Real, N, PointPolicy>::insertRange(
		const Input_Point_ConstRange& pointSet)
	{
		// Insert the points, and get rid of the reported iterators.
		// Note: the reporting is optimized away entirely.
		insertRange(pointSet, Null_Output());
	}

	template <typename Real, int N, typename PointPolicy>
	void PointKdTree<Real, N, PointPolicy>::erase(
		const Point_ConstIterator& iter)
	{
		commitErase(iter);
	}

	template <typename Real, int N, typename PointPolicy>
	template <typename Point_ConstIterator_ConstRange>
	void PointKdTree<Real, N, PointPolicy>::erase(
		const Point_ConstIterator_ConstRange& pointSet)
	{
		typedef typename boost::range_iterator<Point_ConstIterator_ConstRange>::type
			Point_ConstIterator_ConstIterator;
		
		Point_ConstIterator_ConstIterator iter = 
			pointSet.begin();
		const Point_ConstIterator_ConstIterator iterEnd = 
			pointSet.end();

		// We use this technique instead of directly
		// incrementing 'iter', because the iterators in
		// 'pointSet' can be counting iterators to
		// iterators in 'pointSet_'.
		Point_ConstIterator_ConstIterator nextIter;
		while(iter != iterEnd)
		{
			nextIter = iter;
			++nextIter;

			erase(iter);

			iter = nextIter;
		}
	}

	template <typename Real, int N, typename PointPolicy>
	void PointKdTree<Real, N, PointPolicy>::hide()
	{
		setHidden(pointSet_.begin(), pointSet_.end(), true);
		setHidden(insertionSet_.begin(), insertionSet_.end(), true);
		hiddenSet_.splice(hiddenSet_.end(), pointSet_);
		hiddenSet_.splice(hiddenSet_.end(), insertionSet_);

		clearPoints(root_);
	}

	template <typename Real, int N, typename PointPolicy>
	void PointKdTree<Real, N, PointPolicy>::hide(
		const Point_ConstIterator& iter)
	{
		commitHide(iter);
	}

	template <typename Real, int N, typename PointPolicy>
	void PointKdTree<Real, N, PointPolicy>::show()
	{
		Point_ConstIterator iter = hiddenSet_.begin();
		Point_ConstIterator end = hiddenSet_.end();
		while(iter != end)
		{
			Point_ConstIterator nextIter = iter;
			++nextIter;

			commitShow(iter);

			iter = nextIter;
		}
	}

	template <typename Real, int N, typename PointPolicy>
	void PointKdTree<Real, N, PointPolicy>::show(
		const Point_ConstIterator& iter)
	{
		// On immediate updates, just do the
		// showing.
		commitShow(iter);
	}

	template <typename Real, int N, typename PointPolicy>
	void PointKdTree<Real, N, PointPolicy>::clear()
	{
		// This operation is always immediate.

		pointSet_.clear();
		hiddenSet_.clear();
		insertionSet_.clear();
		destructSubtree(root_);
		nodeAllocator_.clear();
		root_ = 0;
		leaves_ = 0;
		bound_ = AlignedBox<Real, N>(
			ofDimension(n()));

		initialize();
	}

	template <typename Real, int N, typename PointPolicy>
	void PointKdTree<Real, N, PointPolicy>::erase(
		bool eraseHidden)
	{
		// This operation is always immediate.

		if (eraseHidden)
		{
			// Remove all points in the hidden set.
			hiddenSet_.clear();
		}

		// Remove the visible points.
		pointSet_.clear();

		// Remove the points in the insertion set.
		insertionSet_.clear();

		// Clear the point ranges in the subtree.
		clearPoints(root_);
	}

	template <typename Real, int N, typename PointPolicy>
	void PointKdTree<Real, N, PointPolicy>::erase(
		const Cursor& cursor, bool eraseHidden)
	{
		// We take the optimization for the case 
		// cursor == root() inside this function.
		erase(cursor.node_, eraseHidden);
	}

	template <typename Real, int N, typename PointPolicy>
	void PointKdTree<Real, N, PointPolicy>::merge()
	{
		// This operation is always immediate.
		
		// We can merge all of the nodes faster and
		// more storage-efficiently than using
		// merge(root_).

		const integer allPoints = points();

		destructSubtree(root_);
		nodeAllocator_.clear();

		Point_ConstIterator last = pointSet_.end();
		--last;

		root_ = allocateLeaf(
			0, pointSet_.begin(), last, allPoints);
		leaves_ = 1;

		setLeaf(pointSet_.begin(), pointSet_.end(), root_);
		setLeaf(hiddenSet_.begin(), hiddenSet_.end(), root_);
	}

	template <typename Real, int N, typename PointPolicy>
	void PointKdTree<Real, N, PointPolicy>::merge(
		const Cursor& cursor)
	{
		// We take the optimization for the case 
		// cursor == root() inside this function.
		merge(cursor.node_);
	}

}

#endif
