// Description: Point kd-tree on a windowed point-set

#ifndef PASTELGEOMETRY_WINDOWED_POINTTREE_H
#define PASTELGEOMETRY_WINDOWED_POINTTREE_H

#include "pastel/geometry/pointkdtree.h"
#include "pastel/geometry/alignedbox.h"

#include <unordered_map>

namespace Pastel
{

	class PointQuery_WindowedPointTree_Concept
	{
	public:
		using Locator = UserDefinedType;

		// The compile-time dimension of the window.
		// If M != Dynamic, M == queryDimension().
		static PASTEL_CONSTEXPR int M = UserDefinedInteger;

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

		static PASTEL_CONSTEXPR int N = Locator::N;
		static PASTEL_CONSTEXPR int M = PointQuery::M;

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

#include "pastel/geometry/windowed_pointtree.hpp"

#endif
