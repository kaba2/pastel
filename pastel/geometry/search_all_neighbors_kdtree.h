// Description: Nearest neighbor searching using a kd-tree
// Documentation: nearest_neighbors.txt

#ifndef PASTEL_SEARCH_ALL_NEIGHBORS_KDTREE_H
#define PASTEL_SEARCH_ALL_NEIGHBORS_KDTREE_H

#include "pastel/sys/point.h"
#include "pastel/sys/array.h"

#include <vector>

namespace Pastel
{

	//! Finds all-k-nearest-neighbours of a point set.
	/*!
	Preconditions:
	kNearestBegin >= 0
	kNearestEnd < pointSet.size()
	kNearestBegin <= kNearestEnd
	maxDistance >= 0
	maxRelativeError >= 0
	maxPointsPerNode >= 1

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

	maxPointsPerNode:
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

	template <int N, typename Real, typename NormBijection, 
		typename ConstIndexIterator, typename SplitRule>
	void searchAllNeighborsKdTree(
		const std::vector<Point<N, Real> >& pointSet,
		const ConstIndexIterator& indexSetBegin,
		const ConstIndexIterator& indexSetEnd,
		integer kNearestBegin,
		integer kNearestEnd,
		const PASTEL_NO_DEDUCTION(Real)& maxDistance,
		const PASTEL_NO_DEDUCTION(Real)& maxRelativeError,
		const NormBijection& normBijection,
		integer maxPointsPerNode,
		const SplitRule& splitRule,
		Array<2, integer>* nearestArray,
		Array<2, Real>* distanceArray = 0);

}

#include "pastel/geometry/search_all_neighbors_kdtree.hpp"

#endif
