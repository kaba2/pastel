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
		typedef UserDefinedType PointPolicy;

		// The compile-time dimension of the window.
		// If M != Dynamic, M == queryDimension().
		static PASTEL_CONSTEXPR int M = UserDefinedInteger;

		const integer queryDimension() const;
		const PointPolicy& pointPolicy() const;

		template <typename Point_Functor>
		void queryPoints(
			const AlignedBox<integer, M>& region,
			Point_Functor report);
	};

	template <typename PointQuery>
	class WindowedPointTree
	{
	public:
		typedef typename PointQuery::PointPolicy PointPolicy;
		typedef typename PointPolicy::Point Point;
		typedef typename PointPolicy::Real Real;

		static PASTEL_CONSTEXPR int N = PointPolicy::N;
		static PASTEL_CONSTEXPR int M = PointQuery::M;

		typedef PointKdTree<Real, N, PointPolicy> Tree;
		typedef typename Tree::Point_ConstIterator ConstIterator;

		typedef std::unordered_map<Point, ConstIterator> ActiveSet;
		typedef typename ActiveSet::iterator ActiveIterator;
		typedef typename ActiveSet::iterator ConstActiveIterator;

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
