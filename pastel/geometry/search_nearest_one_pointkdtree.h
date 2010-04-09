// Description: Nearest neighbor searching for PointKdTree
// Documentation: nearest_neighbors.txt

#ifndef PASTEL_SEARCH_NEAREST_ONE_POINTKDTREE_H
#define PASTEL_SEARCH_NEAREST_ONE_POINTKDTREE_H

#include "pastel/geometry/pointkdtree.h"
#include "pastel/geometry/acceptpoint_concept.h"

#include "pastel/math/normbijection_concept.h"

#include "pastel/sys/keyvalue.h"

namespace Pastel
{

	//! Finds nearest neighbors for a point in a kdTree.
	/*!
	Preconditions:
	maxDistance >= 0
	maxRelativeError >= 0
	*/
	template <typename Real, int N, typename PointPolicy, 
		typename SearchPoint, typename AcceptPoint, 
		typename NormBijection, typename SearchAlgorithm>
	KeyValue<Real, typename PointKdTree<Real, N, PointPolicy>::ConstObjectIterator>
		searchNearestOne(
		const PointKdTree<Real, N, PointPolicy>& kdTree,
		const SearchPoint& searchPoint,
		const PASTEL_NO_DEDUCTION(Real)& maxDistance,
		const PASTEL_NO_DEDUCTION(Real)& maxRelativeError,
		const AcceptPoint& acceptPoint,
		integer bucketSize,
		const NormBijection& normBijection,
		const SearchAlgorithm& searchAlgorithm);

	//! Finds nearest neighbors for a point in a kdTree.
	/*!
	This is a convenience function that calls
	searchNearestOne(
		kdTree, searchPoint,
		maxDistance, maxRelativeError,
		acceptPoint, bucketSize,
		normBijection,
		DepthFirst_SearchAlgorithm_PointKdTree());
	*/
	template <typename Real, int N, typename PointPolicy, 
		typename SearchPoint, typename AcceptPoint, 
		typename NormBijection>
	KeyValue<Real, typename PointKdTree<Real, N, PointPolicy>::ConstObjectIterator>
		searchNearestOne(
		const PointKdTree<Real, N, PointPolicy>& kdTree,
		const SearchPoint& searchPoint,
		const PASTEL_NO_DEDUCTION(Real)& maxDistance,
		const PASTEL_NO_DEDUCTION(Real)& maxRelativeError,
		const AcceptPoint& acceptPoint,
		integer bucketSize,
		const NormBijection& normBijection);

	//! Finds nearest neighbors for a point in a kdTree.
	/*!
	This is a convenience function that calls
	searchNearestOne(
		kdTree, searchPoint,
		maxDistance, maxRelativeError,
		acceptPoint, 
		bucketSize,
		Euclidean_NormBijection<Real>());
	*/
	template <typename Real, int N, typename PointPolicy, 
		typename SearchPoint, typename AcceptPoint>
	KeyValue<Real, typename PointKdTree<Real, N, PointPolicy>::ConstObjectIterator>
		searchNearestOne(
		const PointKdTree<Real, N, PointPolicy>& kdTree,
		const SearchPoint& searchPoint,
		const PASTEL_NO_DEDUCTION(Real)& maxDistance,
		const PASTEL_NO_DEDUCTION(Real)& maxRelativeError,
		const AcceptPoint& acceptPoint,
		integer bucketSize);

	//! Finds nearest neighbors for a point in a kdTree.
	/*!
	This is a convenience function that calls
	searchNearestOne(
		kdTree, searchPoint,
		maxDistance, maxRelativeError,
		acceptPoint, 
		1);
	*/
	template <typename Real, int N, typename PointPolicy, 
		typename SearchPoint, typename AcceptPoint>
	KeyValue<Real, typename PointKdTree<Real, N, PointPolicy>::ConstObjectIterator>
		searchNearestOne(
		const PointKdTree<Real, N, PointPolicy>& kdTree,
		const SearchPoint& searchPoint,
		const PASTEL_NO_DEDUCTION(Real)& maxDistance,
		const PASTEL_NO_DEDUCTION(Real)& maxRelativeError,
		const AcceptPoint& acceptPoint);

	//! Finds nearest neighbors for a point in a kdTree.
	/*!
	This is a convenience function that calls
	searchNearestOne(
		kdTree, searchPoint,
		maxDistance, maxRelativeError,
		Always_AcceptPoint<typename PointKdTree<Real, N, PointPolicy>::ConstObjectIterator>());
	*/
	template <typename Real, int N, typename PointPolicy,
		typename SearchPoint>
	KeyValue<Real, typename PointKdTree<Real, N, PointPolicy>::ConstObjectIterator>
		searchNearestOne(
		const PointKdTree<Real, N, PointPolicy>& kdTree,
		const SearchPoint& searchPoint,
		const PASTEL_NO_DEDUCTION(Real)& maxDistance,
		const PASTEL_NO_DEDUCTION(Real)& maxRelativeError);

	//! Finds nearest neighbors for a point in a kdTree.
	/*!
	This is a convenience function that calls
	searchNearestOne(
		kdTree, searchPoint,
		maxDistance, maxRelativeError,
		Dont_AcceptPoint<typename PointKdTree<Real, N, PointPolicy>::ConstObjectIterator>(searchPoint));
	*/
	template <typename Real, int N, typename PointPolicy>
	KeyValue<Real, typename PointKdTree<Real, N, PointPolicy>::ConstObjectIterator>
		searchNearestOne(
		const PointKdTree<Real, N, PointPolicy>& kdTree,
		const typename PointKdTree<Real, N, PointPolicy>::ConstObjectIterator& searchPoint,
		const PASTEL_NO_DEDUCTION(Real)& maxDistance,
		const PASTEL_NO_DEDUCTION(Real)& maxRelativeError);

	//! Finds nearest neighbors for a point in a kdTree.
	/*!
	This is a convenience function that calls
	searchNearestOne(
		kdTree, searchPoint,
		maxDistance, 0);
	*/
	template <typename Real, int N, typename PointPolicy,
		typename SearchPoint>
	KeyValue<Real, typename PointKdTree<Real, N, PointPolicy>::ConstObjectIterator>
		searchNearestOne(
		const PointKdTree<Real, N, PointPolicy>& kdTree,
		const SearchPoint& searchPoint,
		const PASTEL_NO_DEDUCTION(Real)& maxDistance);

	//! Finds nearest neighbors for a point in a kdTree.
	/*!
	This is a convenience function that calls
	searchNearestOne(
		kdTree, searchPoint,
		infinity<Real>());
	*/
	template <typename Real, int N, typename PointPolicy,
		typename SearchPoint>
	KeyValue<Real, typename PointKdTree<Real, N, PointPolicy>::ConstObjectIterator>
		searchNearestOne(
		const PointKdTree<Real, N, PointPolicy>& kdTree,
		const SearchPoint& searchPoint);

}

#include "pastel/geometry/search_nearest_one_pointkdtree.hpp"

#endif
