#ifndef PASTEL_SEARCH_NEAREST_ONE_POINTKDTREE_HPP
#define PASTEL_SEARCH_NEAREST_ONE_POINTKDTREE_HPP

#include "pastel/geometry/search_nearest_one_pointkdtree.h"
#include "pastel/geometry/search_nearest_pointkdtree.h"
#include "pastel/geometry/always_acceptpoint.h"
#include "pastel/geometry/dont_acceptpoint.h"

#include "pastel/math/euclidean_normbijection.h"

#include <boost/type_traits/is_same.hpp>

namespace Pastel
{

	template <typename Real, int N, typename ObjectPolicy, 
		typename SearchPoint, typename AcceptPoint, 
		typename NormBijection, typename SearchAlgorithm>
	KeyValue<Real, typename PointKdTree<Real, N, ObjectPolicy>::ConstObjectIterator>
		searchNearestOne(
		const PointKdTree<Real, N, ObjectPolicy>& kdTree,
		const SearchPoint& searchPoint,
		const PASTEL_NO_DEDUCTION(Real)& maxDistance,
		const PASTEL_NO_DEDUCTION(Real)& maxRelativeError,
		const AcceptPoint& acceptPoint,
		const NormBijection& normBijection,
		const SearchAlgorithm& searchAlgorithm)
	{
		ENSURE_OP(maxDistance, >=, 0);
		ENSURE_OP(maxRelativeError, >=, 0);

		if (kdTree.empty())
		{
			return keyValue(infinity<Real>(), kdTree.end());
		}

		typedef PointKdTree<Real, N, ObjectPolicy> Tree;
		typedef typename Tree::Cursor Cursor;
		typedef typename Tree::ConstObjectIterator ConstObjectIterator;

		Real distance = 0;
		ConstObjectIterator nearest;

		searchNearest(
			kdTree, searchPoint, 
			1, &nearest, &distance,
			maxDistance, maxRelativeError,
			acceptPoint, normBijection, 
			searchAlgorithm);

		return keyValue(distance, nearest);
	}

	template <typename Real, int N, typename ObjectPolicy, 
		typename SearchPoint, typename AcceptPoint, 
		typename NormBijection>
	KeyValue<Real, typename PointKdTree<Real, N, ObjectPolicy>::ConstObjectIterator>
		searchNearestOne(
		const PointKdTree<Real, N, ObjectPolicy>& kdTree,
		const SearchPoint& searchPoint,
		const PASTEL_NO_DEDUCTION(Real)& maxDistance,
		const PASTEL_NO_DEDUCTION(Real)& maxRelativeError,
		const AcceptPoint& acceptPoint,
		const NormBijection& normBijection)
	{
		return Pastel::searchNearestOne(
			kdTree, searchPoint, 
			maxDistance, maxRelativeError,
			acceptPoint, normBijection,
			BestFirst_SearchAlgorithm_PointKdTree());
	}

	template <typename Real, int N, typename ObjectPolicy, 
		typename SearchPoint, typename AcceptPoint>
	KeyValue<Real, typename PointKdTree<Real, N, ObjectPolicy>::ConstObjectIterator>
		searchNearestOne(
		const PointKdTree<Real, N, ObjectPolicy>& kdTree,
		const SearchPoint& searchPoint,
		const PASTEL_NO_DEDUCTION(Real)& maxDistance,
		const PASTEL_NO_DEDUCTION(Real)& maxRelativeError,
		const AcceptPoint& acceptPoint)
	{
		return Pastel::searchNearestOne(
			kdTree, searchPoint, 
			maxDistance, maxRelativeError,
			acceptPoint, Euclidean_NormBijection<Real>());
	}

	template <typename Real, int N, typename ObjectPolicy, 
		typename SearchPoint>
	KeyValue<Real, typename PointKdTree<Real, N, ObjectPolicy>::ConstObjectIterator>
		searchNearestOne(
		const PointKdTree<Real, N, ObjectPolicy>& kdTree,
		const SearchPoint& searchPoint,
		const PASTEL_NO_DEDUCTION(Real)& maxDistance,
		const PASTEL_NO_DEDUCTION(Real)& maxRelativeError)
	{
		typedef typename PointKdTree<Real, N, ObjectPolicy>::ConstObjectIterator
			ConstObjectIterator;

		return Pastel::searchNearestOne(
			kdTree, searchPoint, 
			maxDistance, maxRelativeError,
			Always_AcceptPoint<ConstObjectIterator>());
	}

	template <typename Real, int N, typename ObjectPolicy>
	KeyValue<Real, typename PointKdTree<Real, N, ObjectPolicy>::ConstObjectIterator>
		searchNearestOne(
		const PointKdTree<Real, N, ObjectPolicy>& kdTree,
		const typename PointKdTree<Real, N, ObjectPolicy>::ConstObjectIterator& searchPoint,
		const PASTEL_NO_DEDUCTION(Real)& maxDistance,
		const PASTEL_NO_DEDUCTION(Real)& maxRelativeError)
	{
		typedef typename PointKdTree<Real, N, ObjectPolicy>::ConstObjectIterator
			ConstObjectIterator;

		return Pastel::searchNearestOne(
			kdTree, searchPoint, 
			maxDistance, maxRelativeError,
			Dont_AcceptPoint<ConstObjectIterator>(searchPoint));
	}

	template <typename Real, int N, typename ObjectPolicy, 
		typename SearchPoint>
	KeyValue<Real, typename PointKdTree<Real, N, ObjectPolicy>::ConstObjectIterator>
		searchNearestOne(
		const PointKdTree<Real, N, ObjectPolicy>& kdTree,
		const SearchPoint& searchPoint,
		const PASTEL_NO_DEDUCTION(Real)& maxDistance)
	{
		return Pastel::searchNearestOne(
			kdTree, searchPoint, 
			maxDistance, 0);
	}

	template <typename Real, int N, typename ObjectPolicy, 
		typename SearchPoint>
	KeyValue<Real, typename PointKdTree<Real, N, ObjectPolicy>::ConstObjectIterator>
		searchNearestOne(
		const PointKdTree<Real, N, ObjectPolicy>& kdTree,
		const SearchPoint& searchPoint)
	{
		return Pastel::searchNearestOne(
			kdTree, searchPoint, 
			infinity<Real>());
	}

}

#endif
