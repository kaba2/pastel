// Description: Nearest neighbor searching for PointKdTree
// Documentation: nearest_neighbors.txt

#ifndef PASTELGEOMETRY_SEARCH_NEAREST_ONE_POINTKDTREE_H
#define PASTELGEOMETRY_SEARCH_NEAREST_ONE_POINTKDTREE_H

#include "pastel/geometry/pointkdtree.h"
#include "pastel/sys/indicator_concept.h"
#include "pastel/math/normbijection_concept.h"

#include "pastel/geometry/depthfirst_searchalgorithm_pointkdtree.h"
#include "pastel/sys/all_indicator.h"
#include "pastel/sys/allexcept_indicator.h"

#include "pastel/math/euclidean_normbijection.h"

#include "pastel/sys/keyvalue.h"

#include "pastel/geometry/search_nearest_one_pointkdtree.hpp"

namespace Pastel
{

	//! Finds the nearest neighbor for a point in a PointKdTree.
	/*!
	kdTree:
	The PointKdTree to search neighbors in.

	searchPoint:
	The point for which to search a neighbor for.
	This can be either a Vector<Real, N>, or
	a Point_ConstIterator of 'kdTree'.
	
	acceptPoint:
	A functor that determines whether to accept a point
	as a neighbor or not. See 'indicators.txt'.

	normBijection:
	The norm used to measure distance.
	See 'pastel/math/normbijection.txt'.

	searchAlgorithm:
	The search algorithm to use for searching the 'kdTree'.
	See 'searchalgorithm_pointkdtree.txt'.

	Optional arguments
	------------------

	maxDistance (>= 0):
	The distance after which points are not considered neighbors
	anymore. Can be set to infinity<Real>(). This distance
	is in terms of the used norm bijection.
	Default: infinity<Real>()

	maxRelativeError (>= 0):
	Maximum allowed relative error in the distance of the  
	result point to the true nearest neighbor. Allowing error
	increases performance. Use 0 for exact matches. 
	Default: 0

	bucketSize (> 0):
	The number of points under which to start a brute-force
	search in a node.
	Default: 16

	Returns (by implicit conversion)
	--------------------------------

	Real:
	The distance (in terms of the norm bijection) to 
	the nearest neighbor. If no neighbor is found, 
	returns infinity<Real>().

	Point_ConstIterator:
	The nearest neighbor. If no neighbor is found,
	returns kdTree.end().

	KeyValue<Real, Point_ConstIterator>:
	A combination of the previous two.
	*/
	template <
		typename Real, int N, typename PointPolicy, 
		typename SearchPoint, 
		typename Indicator = All_Indicator, 
		typename NormBijection = Euclidean_NormBijection<Real>, 
		typename SearchAlgorithm = DepthFirst_SearchAlgorithm_PointKdTree>
	SearchNearestOne_<Real, N, PointPolicy, SearchPoint, 
		Indicator, NormBijection, SearchAlgorithm>
		searchNearestOne(
		const PointKdTree<Real, N, PointPolicy>& kdTree,
		const SearchPoint& searchPoint,
		const Indicator& acceptPoint = All_Indicator(),
		const NormBijection& normBijection = Euclidean_NormBijection<Real>(),
		const SearchAlgorithm& searchAlgorithm = DepthFirst_SearchAlgorithm_PointKdTree())
	{
		return SearchNearestOne_<Real, N, PointPolicy, SearchPoint, 
			Indicator, NormBijection, SearchAlgorithm>(kdTree, searchPoint,
			acceptPoint, normBijection, searchAlgorithm);
	}

	template <
		typename Real, int N, typename PointPolicy, 
		typename Indicator = AllExcept_Indicator<typename PointKdTree<Real, N, PointPolicy>::Point_ConstIterator>, 
		typename NormBijection = Euclidean_NormBijection<Real>, 
		typename SearchAlgorithm = DepthFirst_SearchAlgorithm_PointKdTree>
	SearchNearestOne_<Real, N, PointPolicy, 
		typename PointKdTree<Real, N, PointPolicy>::Point_ConstIterator, 
		Indicator, NormBijection, SearchAlgorithm>
	searchNearestOne(
		const PointKdTree<Real, N, PointPolicy>& kdTree,
		const typename PointKdTree<Real, N, PointPolicy>::Point_ConstIterator& searchPoint,
		const Indicator& acceptPoint = dontIndicator(searchPoint),
		const NormBijection& normBijection = Euclidean_NormBijection<Real>(),
		const SearchAlgorithm& searchAlgorithm = DepthFirst_SearchAlgorithm_PointKdTree())
	{
		return SearchNearestOne_<Real, N, PointPolicy, 
			typename PointKdTree<Real, N, PointPolicy>::Point_ConstIterator, 
			Indicator, NormBijection, SearchAlgorithm>(kdTree, searchPoint,
			acceptPoint, normBijection, searchAlgorithm);
	}

}

#endif
