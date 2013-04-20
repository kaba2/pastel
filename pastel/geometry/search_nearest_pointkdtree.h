// Description: K-nearest-neighbors searching for PointKdTree
// Documentation: nearest_neighbors.txt

#ifndef PASTELGEOMETRY_SEARCH_NEAREST_POINTKDTREE_H
#define PASTELGEOMETRY_SEARCH_NEAREST_POINTKDTREE_H

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

	nearestOutput:
	A reporter to which the found neighbors 
	(Point_ConstIterator of 'kdTree') are reported to.

	distanceOutput:
	A reporter to which the distances of the found
	neighbors (Real) are reported to.

	maxDistance:
	The distance after which points are not considered neighbors
	anymore. Can be set to infinity<Real>(). The distance is
	in terms of the norm bijection.

	maxRelativeError:
	Maximum allowed relative error in the distance of the k:th 
	result point to the true k:th nearest neighbor. Allowing error
	increases performance. Use 0 for exact matches. 

	acceptPoint:
	A functor that takes in a Point_ConstIterator of 
	the 'kdTree' and returns a bool if the point should be accepted
	as a neighbor or not. See 'acceptpoint.txt'.

	normBijection:
	The norm used to measure distance.
	See 'pastel/math/normbijection.txt'.

	searchAlgorithm:
	The search algorithm to use for searching the 'kdTree'.
	See 'searchalgorithm_pointkdtree.txt'.

	returns:
	The number of found neighbors.
	*/
	template <typename Real, int N, typename PointPolicy, 
		typename SearchPoint, typename Nearest_Output, 
		typename Real_Output, typename AcceptPoint,
		typename NormBijection, typename SearchAlgorithm_PointKdTree>
	integer searchNearest(
		const PointKdTree<Real, N, PointPolicy>& kdTree,
		const SearchPoint& searchPoint,
		integer kNearest,
		const Nearest_Output& nearestOutput,
		const Real_Output& distanceOutput,
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
		kNearest, nearestOutput, distanceOutput,
		maxDistance, maxRelativeError, acceptPoint,
		normBijection,
		DepthFirst_SearchAlgorithm_PointKdTree());
	*/
	template <typename Real, int N, typename PointPolicy, 
		typename SearchPoint, typename Nearest_Output, 
		typename Real_Output, typename AcceptPoint,
		typename NormBijection>
	integer searchNearest(
		const PointKdTree<Real, N, PointPolicy>& kdTree,
		const SearchPoint& searchPoint,
		integer kNearest,
		const Nearest_Output& nearestOutput,
		const Real_Output& distanceOutput,
		const PASTEL_NO_DEDUCTION(Real)& maxDistance,
		const PASTEL_NO_DEDUCTION(Real)& maxRelativeError,
		const AcceptPoint& acceptPoint,
		integer bucketSize,
		const NormBijection& normBijection);

	//! Finds nearest neighbors for a point in a kdTree.
	/*!
	This is a convenience function that calls:
	searchNearest(kdTree, searchPoint,
		kNearest, nearestOutput, distanceOutput,
		maxDistance, maxRelativeError, acceptPoint,
		bucketSize,	Euclidean_NormBijection<Real>());
	*/
	template <typename Real, int N, typename PointPolicy, 
		typename SearchPoint, typename Nearest_Output, 
		typename Real_Output, typename AcceptPoint>
	integer searchNearest(
		const PointKdTree<Real, N, PointPolicy>& kdTree,
		const SearchPoint& searchPoint,
		integer kNearest,
		const Nearest_Output& nearestOutput,
		const Real_Output& distanceOutput,
		const PASTEL_NO_DEDUCTION(Real)& maxDistance,
		const PASTEL_NO_DEDUCTION(Real)& maxRelativeError,
		const AcceptPoint& acceptPoint,
		integer bucketSize);

	//! Finds nearest neighbors for a point in a kdTree.
	/*!
	This is a convenience function that calls:
	searchNearest(kdTree, searchPoint,
		kNearest, nearestOutput, distanceOutput,
		maxDistance, maxRelativeError, acceptPoint, 1);
	*/
	template <typename Real, int N, typename PointPolicy, 
		typename SearchPoint, typename Nearest_Output, 
		typename Real_Output, typename AcceptPoint>
	integer searchNearest(
		const PointKdTree<Real, N, PointPolicy>& kdTree,
		const SearchPoint& searchPoint,
		integer kNearest,
		const Nearest_Output& nearestOutput,
		const Real_Output& distanceOutput,
		const PASTEL_NO_DEDUCTION(Real)& maxDistance,
		const PASTEL_NO_DEDUCTION(Real)& maxRelativeError,
		const AcceptPoint& acceptPoint);

	//! Finds nearest neighbors for a point in a kdTree.
	/*!
	This is a convenience function that calls:
	searchNearest(kdTree, searchPoint,
		kNearest, nearestOutput, distanceOutput,
		maxDistance, maxRelativeError, 
		alwaysAcceptPoint(kdTree));
	*/
	template <typename Real, int N, typename PointPolicy, 
		typename SearchPoint, typename Nearest_Output, 
		typename Real_Output>
	integer searchNearest(
		const PointKdTree<Real, N, PointPolicy>& kdTree,
		const SearchPoint& searchPoint,
		integer kNearest,
		const Nearest_Output& nearestOutput,
		const Real_Output& distanceOutput,
		const PASTEL_NO_DEDUCTION(Real)& maxDistance,
		const PASTEL_NO_DEDUCTION(Real)& maxRelativeError);

	//! Finds nearest neighbors for a point in a kdTree.
	/*!
	This is a convenience function that calls:
	searchNearest(kdTree, searchPoint,
		kNearest, nearestOutput, distanceOutput,
		maxDistance, 0);
	*/
	template <typename Real, int N, typename PointPolicy, 
		typename SearchPoint, typename Nearest_Output, 
		typename Real_Output>
	integer searchNearest(
		const PointKdTree<Real, N, PointPolicy>& kdTree,
		const SearchPoint& searchPoint,
		integer kNearest,
		const Nearest_Output& nearestOutput,
		const Real_Output& distanceOutput,
		const PASTEL_NO_DEDUCTION(Real)& maxDistance);

	//! Finds nearest neighbors for a point in a kdTree.
	/*!
	This is a convenience function that calls:
	searchNearest(kdTree, searchPoint,
		kNearest, nearestOutput, distanceOutput,
		infinity<Real>());
	*/
	template <typename Real, int N, typename PointPolicy, 
		typename SearchPoint, typename Nearest_Output, 
		typename Real_Output>
	integer searchNearest(
		const PointKdTree<Real, N, PointPolicy>& kdTree,
		const SearchPoint& searchPoint,
		integer kNearest,
		const Nearest_Output& nearestOutput,
		const Real_Output& distanceOutput);

}

#include "pastel/geometry/search_nearest_one_pointkdtree.h"

#include "pastel/geometry/search_nearest_pointkdtree.hpp"

#endif
