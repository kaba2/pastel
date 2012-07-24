// Description: Point kd-tree on a windowed point-set

#ifndef PASTEL_WINDOWEDPOINTTREE_H
#define PASTEL_WINDOWEDPOINTTREE_H

#include "pastel/geometry/pointkdtree.h"
#include "pastel/geometry/alignedbox.h"

#include <unordered_map>

namespace Pastel
{

	class PointQuery_WindowedPointTree_Concept
	{
	public:
		typedef UserDefinedType PointPolicy;

		enum
		{
			// The compile-time dimension of the window.
			// If M != Dynamic, M == queryDimension().
			M = UserDefinedInteger
		};

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

		enum
		{
			N = PointPolicy::N,
			M = PointQuery::M
		};

		typedef PointKdTree<Real, N, PointPolicy> Tree;
		typedef typename Tree::Point_ConstIterator ConstIterator;

		typedef std::unordered_map<Point, ConstIterator> ActiveSet;
		typedef typename ActiveSet::iterator ActiveIterator;
		typedef typename ActiveSet::iterator ConstActiveIterator;

		explicit WindowedPointTree(
			const PointQuery& pointQuery = PointQuery());

		void swap(WindowedPointTree& that);

		template <typename SplitRule_PointKdTree>
		void refine(const SplitRule_PointKdTree& splitRule,
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

#include "pastel/geometry/windowedpointtree.hpp"

#endif
