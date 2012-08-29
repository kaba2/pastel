// Description: Nearest neighbor searching for PointKdTree
// Documentation: nearest_neighbors.txt

#ifndef PASTELGEOMETRY_SEARCH_NEAREST_ONE_POINTKDTREE_H
#define PASTELGEOMETRY_SEARCH_NEAREST_ONE_POINTKDTREE_H

#include "pastel/geometry/pointkdtree.h"
#include "pastel/geometry/acceptpoint_concept.h"

#include "pastel/math/normbijection_concept.h"

#include "pastel/sys/keyvalue.h"

namespace Pastel
{

	//! Finds the nearest neighbor for a point in a kdTree.
	/*!
	Preconditions:
	maxDistance >= 0
	maxRelativeError >= 0

	kdTree:
	The PointKdTree to search neighbors in.

	searchPoint:
	The point for which to search a neighbor for.
	This can be either a Vector<Real, N>, or
	a Point_ConstIterator of 'kdTree'.
	
	maxDistance:
	The distance after which points are not considered neighbors
	anymore. Can be set to infinity<Real>(). This distance
	is in terms of the used norm bijection.

	maxRelativeError:
	Maximum allowed relative error in the distance of the  
	result point to the true nearest neighbor. Allowing error
	increases performance. Use 0 for exact matches. 

	acceptPoint:
	A functor that determines whether to accept a point
	as a neighbor or not. See 'acceptpoint.txt'.

	normBijection:
	The norm used to measure distance.
	See 'pastel/math/normbijection.txt'.

	searchAlgorithm:
	The search algorithm to use for searching the 'kdTree'.
	See 'searchalgorithm_pointkdtree.txt'.

	returns:
	A key-value pair, where the key is the distance
	(in terms of the norm-bijection), and the value 
	is a Point_ConstIterator to 'kdTree', denoting
	the nearest neighbor.
	*/
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
		const SearchAlgorithm& searchAlgorithm);

	//! Finds the nearest neighbor for a point in a kdTree.
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
	KeyValue<Real, typename PointKdTree<Real, N, PointPolicy>::Point_ConstIterator>
		searchNearestOne(
		const PointKdTree<Real, N, PointPolicy>& kdTree,
		const SearchPoint& searchPoint,
		const PASTEL_NO_DEDUCTION(Real)& maxDistance,
		const PASTEL_NO_DEDUCTION(Real)& maxRelativeError,
		const AcceptPoint& acceptPoint,
		integer bucketSize,
		const NormBijection& normBijection);

	//! Finds the nearest neighbor for a point in a kdTree.
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
	KeyValue<Real, typename PointKdTree<Real, N, PointPolicy>::Point_ConstIterator>
		searchNearestOne(
		const PointKdTree<Real, N, PointPolicy>& kdTree,
		const SearchPoint& searchPoint,
		const PASTEL_NO_DEDUCTION(Real)& maxDistance,
		const PASTEL_NO_DEDUCTION(Real)& maxRelativeError,
		const AcceptPoint& acceptPoint,
		integer bucketSize);

	//! Finds the nearest neighbor for a point in a kdTree.
	/*!
	This is a convenience function that calls
	searchNearestOne(
		kdTree, searchPoint,
		maxDistance, maxRelativeError,
		acceptPoint, 
		16);
	*/
	template <typename Real, int N, typename PointPolicy, 
		typename SearchPoint, typename AcceptPoint>
	KeyValue<Real, typename PointKdTree<Real, N, PointPolicy>::Point_ConstIterator>
		searchNearestOne(
		const PointKdTree<Real, N, PointPolicy>& kdTree,
		const SearchPoint& searchPoint,
		const PASTEL_NO_DEDUCTION(Real)& maxDistance,
		const PASTEL_NO_DEDUCTION(Real)& maxRelativeError,
		const AcceptPoint& acceptPoint);

	//! Finds the nearest neighbor for a point in a kdTree.
	/*!
	This is a convenience function that calls
	searchNearestOne(
		kdTree, searchPoint,
		maxDistance, maxRelativeError,
		alwaysAcceptPoint(kdTree));
	*/
	template <typename Real, int N, typename PointPolicy,
		typename SearchPoint>
	KeyValue<Real, typename PointKdTree<Real, N, PointPolicy>::Point_ConstIterator>
		searchNearestOne(
		const PointKdTree<Real, N, PointPolicy>& kdTree,
		const SearchPoint& searchPoint,
		const PASTEL_NO_DEDUCTION(Real)& maxDistance,
		const PASTEL_NO_DEDUCTION(Real)& maxRelativeError);

	//! Finds the nearest neighbor for a point in a kdTree.
	/*!
	This is a convenience function that calls
	searchNearestOne(
		kdTree, searchPoint,
		maxDistance, maxRelativeError,
		dontAcceptPoint(searchPoint));
	*/
	template <typename Real, int N, typename PointPolicy>
	KeyValue<Real, typename PointKdTree<Real, N, PointPolicy>::Point_ConstIterator>
		searchNearestOne(
		const PointKdTree<Real, N, PointPolicy>& kdTree,
		const typename PointKdTree<Real, N, PointPolicy>::Point_ConstIterator& searchPoint,
		const PASTEL_NO_DEDUCTION(Real)& maxDistance,
		const PASTEL_NO_DEDUCTION(Real)& maxRelativeError);

	//! Finds the nearest neighbor for a point in a kdTree.
	/*!
	This is a convenience function that calls
	searchNearestOne(
		kdTree, searchPoint,
		maxDistance, 0);
	*/
	template <typename Real, int N, typename PointPolicy,
		typename SearchPoint>
	KeyValue<Real, typename PointKdTree<Real, N, PointPolicy>::Point_ConstIterator>
		searchNearestOne(
		const PointKdTree<Real, N, PointPolicy>& kdTree,
		const SearchPoint& searchPoint,
		const PASTEL_NO_DEDUCTION(Real)& maxDistance);

	//! Finds the nearest neighbor for a point in a kdTree.
	/*!
	This is a convenience function that calls
	searchNearestOne(
		kdTree, searchPoint,
		infinity<Real>());
	*/
	template <typename Real, int N, typename PointPolicy,
		typename SearchPoint>
	KeyValue<Real, typename PointKdTree<Real, N, PointPolicy>::Point_ConstIterator>
		searchNearestOne(
		const PointKdTree<Real, N, PointPolicy>& kdTree,
		const SearchPoint& searchPoint);

}

#include "pastel/geometry/search_nearest_one_pointkdtree.hpp"

#endif
