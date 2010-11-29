// Description: K-nearest-neighbors searching for PointKdTree
// Documentation: nearest_neighbors.txt

#ifndef PASTEL_SEARCH_NEAREST_POINTKDTREE_H
#define PASTEL_SEARCH_NEAREST_POINTKDTREE_H

#include "pastel/geometry/pointkdtree.h"
#include "pastel/geometry/acceptpoint_concept.h"

#include "pastel/math/normbijection_concept.h"

#include "pastel/sys/keyvalue.h"

namespace Pastel
{

	//! Finds k nearest neighbors for a point in a PointKdTree.
	/*!
	Preconditions:
	maxDistance >= 0
	maxRelativeError >= 0
	kNearest >= 0

	kdTree:
	The PointKdTree to search neighbors in.

	searchPoint:
	The point for which to search a neighbor for.
	This can be either a Vector<Real, N>, or
	a Point_ConstIterator of 'kdTree'.
	
	kNearest:
	The number of nearest neighbors to search.

	nearestBegin:
	An output iterator to which the found neighbors 
	(Point_ConstIterator of 'kdTree') are output to.

	distanceBegin:
	An output iterator to which the distance of the found
	neighbors (Real) are output to.

	maxDistance:
	The distance after which points are not considered neighbors
	anymore. Can be set to infinity<Real>().

	maxRelativeError:
	Maximum allowed relative error in the distance of the k:th 
	result point to the true k:th nearest neighbor. Allowing error
	increases performance. Use 0 for exact matches. 

	acceptPoint:
	A functor that takes in a Point_ConstIterator of 
	the 'kdTree' and returns a bool if the point should be accepted
	as a neighbor or not. Default construct Always_AcceptPoint class
	to accept all candidates. Default construct Dont_AcceptPoint
	to reject a specific Point_ConstIterator of the 'kdTree'. 
	Construct PointDont_AcceptPoint to reject a specific 
	Point of the 'kdTree'.

	normBijection:
	Defines the norm used to measure distance.
	See "pastel/math/normbijections.h" for predefined norm bijections.

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
	The number of found neighbors.
	*/
	template <typename Real, int N, typename PointPolicy, 
		typename SearchPoint, typename NearestIterator, 
		typename DistanceIterator, typename AcceptPoint,
		typename NormBijection, typename SearchAlgorithm_PointKdTree>
	integer searchNearest(
		const PointKdTree<Real, N, PointPolicy>& kdTree,
		const SearchPoint& searchPoint,
		integer kNearest,
		const NearestIterator& nearestBegin,
		const DistanceIterator& distanceBegin,
		const PASTEL_NO_DEDUCTION(Real)& maxDistance,
		const PASTEL_NO_DEDUCTION(Real)& maxRelativeError,
		const AcceptPoint& acceptPoint,
		integer bucketSize,
		const NormBijection& normBijection,
		const SearchAlgorithm_PointKdTree& searchAlgorithm);

	//! Finds nearest neighbors for a point in a kdTree.
	/*!
	This is a convenience function that calls:
	searchNearest(kdTree, searchPoint,
		kNearest, nearestBegin, distanceBegin,
		maxDistance, maxRelativeError, acceptPoint,
		normBijection,
		DepthFirst_SearchAlgorithm_PointKdTree());
	*/
	template <typename Real, int N, typename PointPolicy, 
		typename SearchPoint, typename NearestIterator, 
		typename DistanceIterator, typename AcceptPoint,
		typename NormBijection>
	integer searchNearest(
		const PointKdTree<Real, N, PointPolicy>& kdTree,
		const SearchPoint& searchPoint,
		integer kNearest,
		const NearestIterator& nearestBegin,
		const DistanceIterator& distanceBegin,
		const PASTEL_NO_DEDUCTION(Real)& maxDistance,
		const PASTEL_NO_DEDUCTION(Real)& maxRelativeError,
		const AcceptPoint& acceptPoint,
		integer bucketSize,
		const NormBijection& normBijection);

	//! Finds nearest neighbors for a point in a kdTree.
	/*!
	This is a convenience function that calls:
	searchNearest(kdTree, searchPoint,
		kNearest, nearestBegin, distanceBegin,
		maxDistance, maxRelativeError, acceptPoint,
		bucketSize,	Euclidean_NormBijection<Real>());
	*/
	template <typename Real, int N, typename PointPolicy, 
		typename SearchPoint, typename NearestIterator, 
		typename DistanceIterator, typename AcceptPoint>
	integer searchNearest(
		const PointKdTree<Real, N, PointPolicy>& kdTree,
		const SearchPoint& searchPoint,
		integer kNearest,
		const NearestIterator& nearestBegin,
		const DistanceIterator& distanceBegin,
		const PASTEL_NO_DEDUCTION(Real)& maxDistance,
		const PASTEL_NO_DEDUCTION(Real)& maxRelativeError,
		const AcceptPoint& acceptPoint,
		integer bucketSize);

	//! Finds nearest neighbors for a point in a kdTree.
	/*!
	This is a convenience function that calls:
	searchNearest(kdTree, searchPoint,
		kNearest, nearestBegin, distanceBegin,
		maxDistance, maxRelativeError, acceptPoint, 1);
	*/
	template <typename Real, int N, typename PointPolicy, 
		typename SearchPoint, typename NearestIterator, 
		typename DistanceIterator, typename AcceptPoint>
	integer searchNearest(
		const PointKdTree<Real, N, PointPolicy>& kdTree,
		const SearchPoint& searchPoint,
		integer kNearest,
		const NearestIterator& nearestBegin,
		const DistanceIterator& distanceBegin,
		const PASTEL_NO_DEDUCTION(Real)& maxDistance,
		const PASTEL_NO_DEDUCTION(Real)& maxRelativeError,
		const AcceptPoint& acceptPoint);

	//! Finds nearest neighbors for a point in a kdTree.
	/*!
	This is a convenience function that calls:
	searchNearest(kdTree, searchPoint,
		kNearest, nearestBegin, distanceBegin,
		maxDistance, maxRelativeError, 
		Always_AcceptPoint<typename PointKdTree<Real, N, PointPolicy>::Point_ConstIterator>());
	*/
	template <typename Real, int N, typename PointPolicy, 
		typename SearchPoint, typename NearestIterator, 
		typename DistanceIterator>
	integer searchNearest(
		const PointKdTree<Real, N, PointPolicy>& kdTree,
		const SearchPoint& searchPoint,
		integer kNearest,
		const NearestIterator& nearestBegin,
		const DistanceIterator& distanceBegin,
		const PASTEL_NO_DEDUCTION(Real)& maxDistance,
		const PASTEL_NO_DEDUCTION(Real)& maxRelativeError);

	//! Finds nearest neighbors for a point in a kdTree.
	/*!
	This is a convenience function that calls:
	searchNearest(kdTree, searchPoint,
		kNearest, nearestBegin, distanceBegin,
		maxDistance, 0);
	*/
	template <typename Real, int N, typename PointPolicy, 
		typename SearchPoint, typename NearestIterator, 
		typename DistanceIterator>
	integer searchNearest(
		const PointKdTree<Real, N, PointPolicy>& kdTree,
		const SearchPoint& searchPoint,
		integer kNearest,
		const NearestIterator& nearestBegin,
		const DistanceIterator& distanceBegin,
		const PASTEL_NO_DEDUCTION(Real)& maxDistance);

	//! Finds nearest neighbors for a point in a kdTree.
	/*!
	This is a convenience function that calls:
	searchNearest(kdTree, searchPoint,
		kNearest, nearestBegin, distanceBegin,
		infinity<Real>());
	*/
	template <typename Real, int N, typename PointPolicy, 
		typename SearchPoint, typename NearestIterator, 
		typename DistanceIterator>
	integer searchNearest(
		const PointKdTree<Real, N, PointPolicy>& kdTree,
		const SearchPoint& searchPoint,
		integer kNearest,
		const NearestIterator& nearestBegin,
		const DistanceIterator& distanceBegin);

}

#include "pastel/geometry/search_nearest_one_pointkdtree.h"

#include "pastel/geometry/search_nearest_pointkdtree.hpp"

#endif
