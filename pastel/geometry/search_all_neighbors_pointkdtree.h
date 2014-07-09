// Description: All-k-nearest-neighbors searching using PointKdTree

#ifndef PASTELGEOMETRY_SEARCH_ALL_NEIGHBORS_POINTKDTREE_H
#define PASTELGEOMETRY_SEARCH_ALL_NEIGHBORS_POINTKDTREE_H

#include "pastel/sys/vector.h"
#include "pastel/sys/array.h"
#include "pastel/sys/range.h"

#include "pastel/geometry/pointkdtree.h"

#include <vector>

namespace Pastel
{

	//! Finds k nearest-neighbours for queried points in a PointKdTree.
	/*!
	Preconditions:
	kNearestBegin >= 0
	kNearestEnd < pointSet.size()
	kNearestBegin <= kNearestEnd
	maxDistanceSet[i] >= 0
	maxRelativeError >= 0

	kdTree:
	A point kd-tree to do the searching in.

	querySet:
	A random access range of point iterators to 
	'kdTree' to use as query points.

	kNearestBegin, kNearestEnd:
	The indices [kNearestBegin, kNearestEnd[ 
	of the nearest neighbors that should be 
	reported. For example, [1, 3[ reports
	the 2nd and 3rd nearest neighbors.

	nearestArray (output):
	An array of point iterators to 'kdTree'
	such that nearestArray(i, j) contains the 
	(i + 'kNearestBegin'):th nearest neighbor 
	of the point in 'querySet[j]'. 
	Note: Can be given a null pointer in which 
	case reporting this information is skipped.

	distanceArray (output):
	An array of distances such that 'distanceArray(i, j)'
	contains the distance between 'querySet[j]'
	and its (i + 'kNearestBegin'):th nearest neighbor.
	Note: Can be given a null pointer in which case
	reporting this information is skipped.

	maxDistanceSet:
	Distances for each query point after which points 
	aren't considered neighbors. This distance is in 
	terms of the norm bijection. 
	Note: Can be set to infinity.

	maxRelativeError:
	Maximum relative error that is allowed between
	the distance from the query point to the reported 
	k:th nearest neighbor and the distance from the
	query point to the correct k:th nearest neighbor.
	Larger allowed errors allow for enhanced performance,
	particularly so in higher dimensions.
	Note: Zero corresponds to exact matches.

	normBijection:
	The norm bijection to use to define distance.

	searchAlgorithm:
	Search algorithm to use.
	*/
	template <
		typename Settings, template <typename> class Customization,
		typename Point_ConstIterator_RandomAccessRange, 
		typename Locator = typename Settings::Locator,
		typename Real = typename Locator::Real,
		integer N = Locator::N,
		typename Real_RandomAccessRange = ConstantRange<Real>,
		typename NormBijection = Euclidean_NormBijection<Real>,
		typename SearchAlgorithm = DepthFirst_SearchAlgorithm_PointKdTree>
	void searchAllNeighbors(
		const PointKdTree<Settings, Customization>& kdTree,
		const Point_ConstIterator_RandomAccessRange& querySet,
		integer kNearestBegin,
		integer kNearestEnd,
		Array<typename PointKdTree<Settings, Customization>::Point_ConstIterator, 2>* nearestArray,
		Array<PASTEL_NO_DEDUCTION(Real), 2>* distanceArray = nullptr,
		const Real_RandomAccessRange& maxDistanceSet = constantRange(infinity<Real>(), querySet.size()),
		const PASTEL_NO_DEDUCTION(Real)& maxRelativeError = 0,
		const NormBijection& normBijection = NormBijection(),
		const SearchAlgorithm& searchAlgorithm = SearchAlgorithm());

}

#include "pastel/geometry/search_all_neighbors_pointkdtree.hpp"

#endif
