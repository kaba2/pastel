#ifndef PASTEL_WINDOWEDPOINTTREE_HPP
#define PASTEL_WINDOWEDPOINTTREE_HPP

#include "pastel/geometry/windowedpointtree.h"
#include "pastel/geometry/difference_alignedbox_alignedbox.h"

namespace Pastel
{

	template <typename PointQuery>
	WindowedPointTree<PointQuery>::WindowedPointTree(
		const PointQuery& pointQuery)
		: tree_(pointQuery.pointPolicy(), false)
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
	template <typename SplitRule>
	void WindowedPointTree<PointQuery>::refine(
		const SplitRule& splitRule,
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
		auto pointEraser = [&](const Point& point) -> bool
		{
			ActiveIterator iter = activeSet_.find(point);
			PENSURE(iter != activeSet_.end())
				//tree_.erase(iter->second);
				tree_.hide(iter->second);
			//activeSet_.erase(point);

			return true;
		};

		difference(window_, window, 
			[&](const AlignedBox<integer, M>& box) -> bool
		{
			pointQuery_.queryPoints(box, pointEraser);
			return true;
		});

		// The points that are in the new window,
		// but not in the current window, need to be added.

		auto pointInserter = [&](const Point& point)
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
		};

		difference(window, window_, 
			[&](const AlignedBox<integer, M>& box) -> bool
		{
			pointQuery_.queryPoints(box, pointInserter);
			return true;
		});

		// Set the new window.
		window_ = window;
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

}

#endif
