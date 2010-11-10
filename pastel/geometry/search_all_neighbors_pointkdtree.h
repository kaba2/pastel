// Description: All-k-nearest-neighbors searching using PointKdTree

#ifndef PASTEL_SEARCH_ALL_NEIGHBORS_POINTKDTREE_H
#define PASTEL_SEARCH_ALL_NEIGHBORS_POINTKDTREE_H

#include "pastel/sys/vector.h"
#include "pastel/sys/array.h"
#include "pastel/sys/randomaccessrange.h"

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
	template <typename Real, int N, typename PointPolicy,
		typename ConstPointIterator_Iterator, 
		typename Real_Iterator,
		typename NormBijection,
		typename SearchAlgorithm>
	void searchAllNeighbors(
		const PointKdTree<Real, N, PointPolicy>& kdTree,
		const RandomAccessRange<ConstPointIterator_Iterator>& querySet,
		integer kNearestBegin,
		integer kNearestEnd,
		Array<typename PointKdTree<Real, N, PointPolicy>::ConstPointIterator, 2>* nearestArray,
		Array<PASTEL_NO_DEDUCTION(Real), 2>* distanceArray,
		const RandomAccessRange<Real_Iterator>& maxDistanceSet,
		const PASTEL_NO_DEDUCTION(Real)& maxRelativeError,
		const NormBijection& normBijection,
		const SearchAlgorithm& searchAlgorithm);

	//! Finds k nearest-neighbours for the given query points.
	/*!
	This is a convenience function that calls:
	searchAllNeighbors(
		kdTree, querySet,
		kNearestBegin, kNearestEnd,
		nearestArray, distanceArray,
		maxDistanceSet, maxRelativeError,
		Euclidean_NormBijection<Real>(),
		constantRange(infinity<Real>(), querySet.size()),
		maxRelativeError,
		normBijection,
		DepthFirst_SearchAlgorithm_PointKdTree());
	*/
	template <typename Real, int N, typename PointPolicy,
		typename ConstPointIterator_Iterator, 
		typename Real_Iterator,
		typename NormBijection>
	void searchAllNeighbors(
		const PointKdTree<Real, N, PointPolicy>& kdTree,
		const RandomAccessRange<ConstPointIterator_Iterator>& querySet,
		integer kNearestBegin,
		integer kNearestEnd,
		Array<typename PointKdTree<Real, N, PointPolicy>::ConstPointIterator, 2>* nearestArray,
		Array<PASTEL_NO_DEDUCTION(Real), 2>* distanceArray,
		const RandomAccessRange<Real_Iterator>& maxDistanceSet,
		const PASTEL_NO_DEDUCTION(Real)& maxRelativeError,
		const NormBijection& normBijection);

	//! Finds k nearest-neighbours for the given query points.
	/*!
	This is a convenience function that calls:
	searchAllNeighbors(
		kdTree, querySet,
		kNearestBegin, kNearestEnd,
		nearestArray, distanceArray,
		maxDistanceSet, maxRelativeError,
		Euclidean_NormBijection<Real>());
	*/
	template <typename Real, int N, typename PointPolicy,
		typename ConstPointIterator_Iterator, 
		typename Real_Iterator>
	void searchAllNeighbors(
		const PointKdTree<Real, N, PointPolicy>& kdTree,
		const RandomAccessRange<ConstPointIterator_Iterator>& querySet,
		integer kNearestBegin,
		integer kNearestEnd,
		Array<typename PointKdTree<Real, N, PointPolicy>::ConstPointIterator, 2>* nearestArray,
		Array<PASTEL_NO_DEDUCTION(Real), 2>* distanceArray,
		const RandomAccessRange<Real_Iterator>& maxDistanceSet,
		const PASTEL_NO_DEDUCTION(Real)& maxRelativeError);

	//! Finds k nearest-neighbours for the given query points.
	/*!
	This is a convenience function that calls:
	searchAllNeighbors(
		kdTree, querySet,
		kNearestBegin, kNearestEnd,
		nearestArray, distanceArray,
		maxDistanceSet, 0);
	*/
	template <typename Real, int N, typename PointPolicy,
		typename ConstPointIterator_Iterator, 
		typename Real_Iterator>
	void searchAllNeighbors(
		const PointKdTree<Real, N, PointPolicy>& kdTree,
		const RandomAccessRange<ConstPointIterator_Iterator>& querySet,
		integer kNearestBegin,
		integer kNearestEnd,
		Array<typename PointKdTree<Real, N, PointPolicy>::ConstPointIterator, 2>* nearestArray,
		Array<PASTEL_NO_DEDUCTION(Real), 2>* distanceArray,
		const RandomAccessRange<Real_Iterator>& maxDistanceSet);

	//! Finds k nearest-neighbours for the given query points.
	/*!
	This is a convenience function that calls:
	searchAllNeighbors(
		kdTree, querySet,
		kNearestBegin, kNearestEnd,
		nearestArray, distanceArray,
		constantRange(infinity<Real>(), querySet.size()));
	*/
	template <typename Real, int N, typename PointPolicy,
		typename ConstPointIterator_Iterator>
	void searchAllNeighbors(
		const PointKdTree<Real, N, PointPolicy>& kdTree,
		const RandomAccessRange<ConstPointIterator_Iterator>& querySet,
		integer kNearestBegin,
		integer kNearestEnd,
		Array<typename PointKdTree<Real, N, PointPolicy>::ConstPointIterator, 2>* nearestArray,
		Array<PASTEL_NO_DEDUCTION(Real), 2>* distanceArray);

	//! Finds k nearest-neighbours for the given query points.
	/*!
	This is a convenience function that calls:
	searchAllNeighbors(
		kdTree, querySet,
		kNearestBegin, kNearestEnd,
		nearestArray, 0);
	*/
	template <typename Real, int N, typename PointPolicy,
		typename ConstPointIterator_Iterator>
	void searchAllNeighbors(
		const PointKdTree<Real, N, PointPolicy>& kdTree,
		const RandomAccessRange<ConstPointIterator_Iterator>& querySet,
		integer kNearestBegin,
		integer kNearestEnd,
		Array<typename PointKdTree<Real, N, PointPolicy>::ConstPointIterator, 2>* nearestArray);

}

#include "pastel/geometry/search_all_neighbors_pointkdtree.hpp"

#endif
