#ifndef PASTEL_SEARCH_NEAREST_ONE_POINTKDTREE_HPP
#define PASTEL_SEARCH_NEAREST_ONE_POINTKDTREE_HPP

#include "pastel/geometry/search_nearest_one_pointkdtree.h"
#include "pastel/geometry/search_nearest_pointkdtree.h"
#include "pastel/geometry/depthfirst_searchalgorithm_pointkdtree.h"
#include "pastel/geometry/always_acceptpoint.h"
#include "pastel/geometry/dont_acceptpoint.h"

#include "pastel/math/euclidean_normbijection.h"

#include "pastel/sys/single_reporter.h"

#include <boost/type_traits/is_same.hpp>

namespace Pastel
{

	template <typename Real, int N, typename PointPolicy, 
		typename SearchPoint, typename AcceptPoint, 
		typename NormBijection, typename SearchAlgorithm>
	KeyValue<Real, typename PointKdTree<Real, N, PointPolicy>::Point_ConstIterator>
		searchNearestOne(
		const PointKdTree<Real, N, PointPolicy>& kdTree,
		const SearchPoint& searchPoint,
		const PASTEL_NO_DEDUCTION(Real)& maxDistance,
		const PASTEL_NO_DEDUCTION(Real)& maxRelativeError,
		const AcceptPoint& acceptPoint,
		integer bucketSize,
		const NormBijection& normBijection,
		const SearchAlgorithm& searchAlgorithm)
	{
		ENSURE_OP(maxDistance, >=, 0);
		ENSURE_OP(maxRelativeError, >=, 0);
		ENSURE_OP(bucketSize, >=, 1);

		if (kdTree.empty())
		{
			return keyValue(infinity<Real>(), kdTree.end());
		}

		typedef PointKdTree<Real, N, PointPolicy> Tree;
		typedef typename Tree::Cursor Cursor;
		typedef typename Tree::Point_ConstIterator Point_ConstIterator;

		Real distance = infinity<Real>();
		Point_ConstIterator nearest = kdTree.end();

		searchNearest(
			kdTree, searchPoint, 
			1, singleReporter(nearest), 
			singleReporter(distance),
			maxDistance, maxRelativeError,
			acceptPoint, bucketSize,
			normBijection, 
			searchAlgorithm);

		return keyValue(distance, nearest);
	}

	template <typename Real, int N, typename PointPolicy, 
		typename SearchPoint, typename AcceptPoint, 
		typename NormBijection>
	KeyValue<Real, typename PointKdTree<Real, N, PointPolicy>::Point_ConstIterator>
		searchNearestOne(
		const PointKdTree<Real, N, PointPolicy>& kdTree,
		const SearchPoint& searchPoint,
		const PASTEL_NO_DEDUCTION(Real)& maxDistance,
		const PASTEL_NO_DEDUCTION(Real)& maxRelativeError,
		const AcceptPoint& acceptPoint,
		integer bucketSize,
		const NormBijection& normBijection)
	{
		return Pastel::searchNearestOne(
			kdTree, searchPoint, 
			maxDistance, maxRelativeError,
			acceptPoint, bucketSize,
			normBijection,
			DepthFirst_SearchAlgorithm_PointKdTree());
	}

	template <typename Real, int N, typename PointPolicy, 
		typename SearchPoint, typename AcceptPoint>
	KeyValue<Real, typename PointKdTree<Real, N, PointPolicy>::Point_ConstIterator>
		searchNearestOne(
		const PointKdTree<Real, N, PointPolicy>& kdTree,
		const SearchPoint& searchPoint,
		const PASTEL_NO_DEDUCTION(Real)& maxDistance,
		const PASTEL_NO_DEDUCTION(Real)& maxRelativeError,
		const AcceptPoint& acceptPoint,
		integer bucketSize)
	{
		return Pastel::searchNearestOne(
			kdTree, searchPoint, 
			maxDistance, maxRelativeError,
			acceptPoint, 
			bucketSize,
			Euclidean_NormBijection<Real>());
	}

	template <typename Real, int N, typename PointPolicy, 
		typename SearchPoint, typename AcceptPoint>
	KeyValue<Real, typename PointKdTree<Real, N, PointPolicy>::Point_ConstIterator>
		searchNearestOne(
		const PointKdTree<Real, N, PointPolicy>& kdTree,
		const SearchPoint& searchPoint,
		const PASTEL_NO_DEDUCTION(Real)& maxDistance,
		const PASTEL_NO_DEDUCTION(Real)& maxRelativeError,
		const AcceptPoint& acceptPoint)
	{
		return Pastel::searchNearestOne(
			kdTree, searchPoint, 
			maxDistance, maxRelativeError,
			acceptPoint, 16);
	}

	template <typename Real, int N, typename PointPolicy,
		typename SearchPoint>
	KeyValue<Real, typename PointKdTree<Real, N, PointPolicy>::Point_ConstIterator>
		searchNearestOne(
		const PointKdTree<Real, N, PointPolicy>& kdTree,
		const SearchPoint& searchPoint,
		const PASTEL_NO_DEDUCTION(Real)& maxDistance,
		const PASTEL_NO_DEDUCTION(Real)& maxRelativeError)
	{
		typedef typename PointKdTree<Real, N, PointPolicy>::Point_ConstIterator
			Point_ConstIterator;

		return Pastel::searchNearestOne(
			kdTree, searchPoint, 
			maxDistance, maxRelativeError,
			alwaysAcceptPoint(kdTree));
	}

	template <typename Real, int N, typename PointPolicy>
	KeyValue<Real, typename PointKdTree<Real, N, PointPolicy>::Point_ConstIterator>
		searchNearestOne(
		const PointKdTree<Real, N, PointPolicy>& kdTree,
		const typename PointKdTree<Real, N, PointPolicy>::Point_ConstIterator& searchPoint,
		const PASTEL_NO_DEDUCTION(Real)& maxDistance,
		const PASTEL_NO_DEDUCTION(Real)& maxRelativeError)
	{
		typedef typename PointKdTree<Real, N, PointPolicy>::Point_ConstIterator
			Point_ConstIterator;

		return Pastel::searchNearestOne(
			kdTree, searchPoint, 
			maxDistance, maxRelativeError,
			dontAcceptPoint(searchPoint));
	}

	template <typename Real, int N, typename PointPolicy,
		typename SearchPoint>
	KeyValue<Real, typename PointKdTree<Real, N, PointPolicy>::Point_ConstIterator>
		searchNearestOne(
		const PointKdTree<Real, N, PointPolicy>& kdTree,
		const SearchPoint& searchPoint,
		const PASTEL_NO_DEDUCTION(Real)& maxDistance)
	{
		return Pastel::searchNearestOne(
			kdTree, searchPoint, 
			maxDistance, 0);
	}

	template <typename Real, int N, typename PointPolicy,
		typename SearchPoint>
	KeyValue<Real, typename PointKdTree<Real, N, PointPolicy>::Point_ConstIterator>
		searchNearestOne(
		const PointKdTree<Real, N, PointPolicy>& kdTree,
		const SearchPoint& searchPoint)
	{
		return Pastel::searchNearestOne(
			kdTree, searchPoint, 
			infinity<Real>());
	}

}

#endif
