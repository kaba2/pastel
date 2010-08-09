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
	a ConstObjectIterator of 'kdTree'.
	
	maxDistance:
	The distance after which points are not considered neighbors
	anymore. Can be set to infinity<Real>().

	maxRelativeError:
	Maximum allowed relative error in the distance of the  
	result point to the true nearest neighbor. Allowing error
	increases performance. Use 0 for exact matches. 

	acceptPoint:
	A functor that takes in a ConstObjectIterator of 
	the 'kdTree' and returns a bool if the object should be accepted
	as a neighbor or not. Default construct Always_AcceptPoint class
	to accept all candidates. Default construct Dont_AcceptPoint
	to reject a specific ConstObjectIterator of the 'kdTree'. 
	Construct ObjectDont_AcceptPoint to reject a specific 
	Object of the 'kdTree'.

	normBijection:
	Defines the norm used to measure distance.
	See "pastel/math/normbijection.h" for predefined norm bijections.

	searchAlgorithm:
	The search algorithm to use for searching the 'kdTree'.
	The possible algorithms at the moment are
	DepthFirst_SearchAlgorithm_PointKdTree
	in 'depthfirst_searchalgorithm_pointkdtree.h' and
	BestFirst_SearchAlgorithm_PointKdTree
	in 'bestfirst_searchalgorithm_pointkdtree.h'
	Default construct an object of this class as 
	an argument.

	returns:
	A key-value pair, where the key is the distance
	(in terms of the norm-bijection), and the value 
	is a ConstObjectIterator to 'kdTree', denoting
	the nearest neighbor.
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
	KeyValue<Real, typename PointKdTree<Real, N, PointPolicy>::ConstObjectIterator>
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
	KeyValue<Real, typename PointKdTree<Real, N, PointPolicy>::ConstObjectIterator>
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

	//! Finds the nearest neighbor for a point in a kdTree.
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

	//! Finds the nearest neighbor for a point in a kdTree.
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

	//! Finds the nearest neighbor for a point in a kdTree.
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

	//! Finds the nearest neighbor for a point in a kdTree.
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
