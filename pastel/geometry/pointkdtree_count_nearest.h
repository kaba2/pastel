// Description: Ball range counting for point kd-tree
// Documentation: pointkdtree_search_range.txt

#ifndef PASTELGEOMETRY_POINTKDTREE_COUNT_NEAREST_H
#define PASTELGEOMETRY_POINTKDTREE_COUNT_NEAREST_H

#include "pastel/geometry/pointkdtree.h"

#include "pastel/sys/indicator_concept.h"
#include "pastel/math/normbijection_concept.h"

#include "pastel/sys/all_indicator.h"
#include "pastel/math/euclidean_normbijection.h"
#include "pastel/geometry/depthfirst_pointkdtree_searchalgorithm.h"

#include "pastel/geometry/pointkdtree_count_nearest.hpp"

namespace Pastel
{

	//! Counts the number of points in a ball in a PointKdTree.
	/*!
	A point is counted in if it is accepted by 'acceptPoint'
	and its norm-bijection distance is less than or equal to 
	'maxDistance'.

	kdTree:
	The PointKdTree to count the points in.

	searchPoint:
	The center point of the search ball.
	This can be either a Vector<Real, N>, or
	a Point_ConstIterator of 'kdTree'.
	
	acceptPoint:
	An indicator which determines whether to accept a point
	in the search ball as a neighbor or not. 
	See 'pastel/sys/indicators.txt'.

	normBijection:
	The norm used to measure distance.
	See 'pastel/math/normbijection.txt'.

	searchAlgorithm:
	The search algorithm to use for searching the 'kdTree'.
	See 'pastel/geometry/pointkdtree_searchalgorithm.txt'.

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

	integer:
	The number of accepted points in the given the distance.
	*/
	template <
		typename Real, int N, typename PointPolicy, 
		typename SearchPoint,
		typename Indicator = All_Indicator,  
		typename NormBijection = Euclidean_NormBijection<Real>, 
		typename SearchAlgorithm_PointKdTree = DepthFirst_SearchAlgorithm_PointKdTree>
	auto countNearest(
		const PointKdTree<Real, N, PointPolicy>& kdTree,
		const SearchPoint& searchPoint,
		const Indicator& acceptPoint = Indicator(),
		const NormBijection& normBijection = NormBijection(),
		const SearchAlgorithm_PointKdTree& searchAlgorithm = SearchAlgorithm_PointKdTree())
		-> 	CountNearest_<Real, N, PointPolicy, SearchPoint, 
		Indicator, NormBijection, SearchAlgorithm_PointKdTree>
	{
		return CountNearest_<Real, N, PointPolicy, SearchPoint, Indicator, 
			NormBijection, SearchAlgorithm_PointKdTree>(kdTree, searchPoint, acceptPoint,
			normBijection, searchAlgorithm);
	}

}

#endif
