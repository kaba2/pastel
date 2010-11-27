#ifndef PASTEL_WINDOWEDPOINTTREE_HPP
#define PASTEL_WINDOWEDPOINTTREE_HPP

#include "pastel/geometry/windowedpointtree.h"
#include "pastel/geometry/difference_alignedbox_alignedbox.h"

#include <boost/bind.hpp>

namespace Pastel
{

	template <typename PointQuery>
	WindowedPointTree<PointQuery>::WindowedPointTree(
		const PointQuery& pointQuery)
		: tree_(pointQuery.pointPolicy(), false, true)
		, window_()
		, activeSet_()
		, pointQuery_(pointQuery)
	{
	}

	template <typename PointQuery>
	void WindowedPointTree<PointQuery>::swap(
		WindowedPointTree& that)
	{
		using std::swap;

		tree_.swap(that.tree_);
		window_.swap(that.window_);
		activeSet_.swap(that.activeSet_);
		swap(pointQuery_, that.pointQuery_);
	}

	template <typename PointQuery>
	template <typename SplitRule_PointKdTree>
	void WindowedPointTree<PointQuery>::refine(
		const SplitRule_PointKdTree& splitRule,
		integer bucketSize)
	{
		tree_.refine(splitRule, bucketSize);
	}

	template <typename PointQuery>
	void WindowedPointTree<PointQuery>::setWindow(
		const AlignedBox<integer, M>& window)
	{
		// The points that are in the current window,
		// but not in the new window, need to be removed.
		difference(window_, window, 
			boost::bind(&WindowedPointTree::eraseBox, this, _1));

		// The points that are in the new window,
		// but not in the current window, need to be added.
		difference(window, window_, 
			boost::bind(&WindowedPointTree::insertBox, this, _1));

		// Set the new window.
		window_ = window;

		// Update the tree.
		tree_.update();
	}

	template <typename PointQuery>
	const AlignedBox<integer, PointQuery::M>& 
		WindowedPointTree<PointQuery>::window() const
	{
		return window_;
	}

	template <typename PointQuery>
	const typename WindowedPointTree<PointQuery>::Tree& 
		WindowedPointTree<PointQuery>::tree() const
	{
		return tree_;
	}

	template <typename PointQuery>
	const typename WindowedPointTree<PointQuery>::ActiveSet& 
		WindowedPointTree<PointQuery>::activeSet() const
	{
		return activeSet_;
	}

	// Private:

	template <typename PointQuery>
	void WindowedPointTree<PointQuery>::insertBox(
		const AlignedBox<integer, M>& box)
	{
		pointQuery_.queryPoints(
			box, boost::bind(&WindowedPointTree::insertPoint, this, _1));
	}

	template <typename PointQuery>
	void WindowedPointTree<PointQuery>::insertPoint(
		const Point& point)
	{
		ActiveIterator iter = activeSet_.find(point);
		if (iter != activeSet_.end())
		{
			tree_.show(iter->second);
		}
		else
		{
			activeSet_.insert(
				std::make_pair(point,
				tree_.insert(point)));
		}
	}

	template <typename PointQuery>
	void WindowedPointTree<PointQuery>::eraseBox(
		const AlignedBox<integer, M>& box)
	{
		pointQuery_.queryPoints(
			box, boost::bind(&WindowedPointTree::erasePoint, this, _1));
	}

	template <typename PointQuery>
	void WindowedPointTree<PointQuery>::erasePoint(
		const Point& point)
	{
		ActiveIterator iter = activeSet_.find(point);
		PENSURE(iter != activeSet_.end())
		//tree_.erase(iter->second);
		tree_.hide(iter->second);
		//activeSet_.erase(point);
	}

}

#endif
