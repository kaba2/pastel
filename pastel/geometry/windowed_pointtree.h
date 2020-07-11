// Description: Point kd-tree on a windowed point-set

#ifndef PASTELGEOMETRY_WINDOWED_POINTTREE_H
#define PASTELGEOMETRY_WINDOWED_POINTTREE_H

#include "pastel/geometry/pointkdtree/pointkdtree.h"
#include "pastel/geometry/difference/difference_alignedbox_alignedbox.h"
#include "pastel/geometry/shape/alignedbox.h"

#include <unordered_map>

namespace Pastel
{

	class PointQuery_WindowedPointTree_Concept
	{
	public:
		using Locator = UserDefinedType;

		// The compile-time dimension of the window.
		// If M != Dynamic, M == queryDimension().
		static constexpr int M = UserDefinedInteger;

		integer queryDimension() const;

		const Locator& locator() const;

		template <typename Point_Functor>
		void queryPoints(
			const AlignedBox<integer, M>& region,
			Point_Functor report);
	};

	template <typename PointQuery>
	class WindowedPointTree
	{
	public:
		using Locator = typename PointQuery::Locator;
		using Point = typename Locator::Point;
		using Real = typename Locator::Real;

		static constexpr int N = Locator::N;
		static constexpr int M = PointQuery::M;

		using Tree = PointKdTree<Real, N, Locator>;
		using ConstIterator = typename Tree::Point_ConstIterator;

		using ActiveSet = std::unordered_map<Point, ConstIterator>;
		using ActiveIterator = typename ActiveSet::iterator;
		using ConstActiveIterator = typename ActiveSet::iterator;

		explicit WindowedPointTree(
			const PointQuery& pointQuery = PointQuery());

		void swap(WindowedPointTree& that);

		template <typename SplitRule>
		void refine(const SplitRule& splitRule,
			integer bucketSize = 8);

		void setWindow(
			const AlignedBox<integer, M>& window);

		const AlignedBox<integer, PointQuery::M>& window() const;

		const Tree& tree() const;

		const ActiveSet& activeSet() const;

	private:
		Tree tree_;
		AlignedBox<integer, M> window_;
		ActiveSet activeSet_;
		PointQuery pointQuery_;
	};

}

namespace Pastel
{

	template <typename PointQuery>
	WindowedPointTree<PointQuery>::WindowedPointTree(
		const PointQuery& pointQuery)
		: tree_(pointQuery.locator(), false)
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
