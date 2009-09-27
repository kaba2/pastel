// Description: All-nearest-neighbors searching using PointKdTree

#ifndef PASTEL_SEARCH_ALL_NEIGHBORS_POINTKDTREE_H
#define PASTEL_SEARCH_ALL_NEIGHBORS_POINTKDTREE_H

#include "pastel/sys/vector.h"
#include "pastel/sys/array.h"
#include "pastel/sys/randomaccessrange.h"

#include "pastel/geometry/pointkdtree.h"

#include <vector>

namespace Pastel
{

	//! Finds k nearest-neighbours for all points.
	/*!
	Preconditions:
	kNearestBegin >= 0
	kNearestEnd < pointSet.size()
	kNearestBegin <= kNearestEnd
	maxDistance >= 0
	maxRelativeError >= 0
	bucketSize >= 1

	pointSet:
	The set of points to do the searching in.

	indexSetBegin, indexSetEnd:
	A sequence of integers denoting indices of those
	points in the 'pointSet' for which the neighbors
	are sought for. Note: use the CountingIterator 
	class for generating a range of integer values 
	without using any additional storage.

	kNearest:
	The number of nearest neighbors to seek for.

	maxDistance:
	A distance after which points aren't considered
	neighbors. This distance is in terms of the
	norm bijection. Note: Can be set to infinity.

	normBijection:
	The norm bijection to use to define distance.

	bucketSize:
	The maximum number of points for a node to not
	to be split.

	splitRule:
	The split rule to use when building the kd-tree.

	nearestArray (output):
	An array of indices such that a(i, j) contains
	the j:th neighbor of the i:th point in the index set.
	Note: Can be given a null pointer in which case
	reporting this information is skipped.

	distanceArray (output):
	An array of distances such that d(i, j) contains
	the distance between i:th point in the index set
	and its j:th neighbor.
	Note: Can be given a null pointer in which case
	reporting this information is skipped.
	*/

	template <typename Real, int N, typename ObjectPolicy,
		typename SearchAlgorithm,
		typename ConstObjectIterator_Iterator, 
		typename Real_Iterator,
		typename NormBijection>
	void searchAllNeighbors(
		const PointKdTree<Real, N, ObjectPolicy>& kdTree,
		const SearchAlgorithm& searchAlgorithm,
		const RandomAccessRange<ConstObjectIterator_Iterator>& querySet,
		integer kNearestBegin,
		integer kNearestEnd,
		const RandomAccessRange<Real_Iterator>& maxDistanceSet,
		const PASTEL_NO_DEDUCTION(Real)& maxRelativeError,
		const NormBijection& normBijection,
		Array<typename PointKdTree<Real, N, ObjectPolicy>::ConstObjectIterator, 2>* nearestArray,
		Array<Real, 2>* distanceArray = 0);

}

#include "pastel/geometry/search_all_neighbors_pointkdtree.hpp"

#endif
