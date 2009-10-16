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

	//! Finds k nearest-neighbours for the given query points.
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

	nearestArray (output):
	An array of indices such that a(i, j) contains
	the i:th neighbor of the j:th point in the index set.
	Note: Can be given a null pointer in which case
	reporting this information is skipped.

	distanceArray (output):
	An array of distances such that d(i, j) contains
	the distance between j:th point in the index set
	and its i:th neighbor.
	Note: Can be given a null pointer in which case
	reporting this information is skipped.
	*/
	template <typename Real, int N, typename ObjectPolicy,
		typename ConstObjectIterator_Iterator, 
		typename Real_Iterator,
		typename NormBijection,
		typename SearchAlgorithm>
	void searchAllNeighbors(
		const PointKdTree<Real, N, ObjectPolicy>& kdTree,
		const RandomAccessRange<ConstObjectIterator_Iterator>& querySet,
		integer kNearestBegin,
		integer kNearestEnd,
		Array<typename PointKdTree<Real, N, ObjectPolicy>::ConstObjectIterator, 2>* nearestArray,
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
		normBijection,
		DepthFirst_SearchAlgorithm_PointKdTree());
	*/
	template <typename Real, int N, typename ObjectPolicy,
		typename ConstObjectIterator_Iterator, 
		typename Real_Iterator,
		typename NormBijection>
	void searchAllNeighbors(
		const PointKdTree<Real, N, ObjectPolicy>& kdTree,
		const RandomAccessRange<ConstObjectIterator_Iterator>& querySet,
		integer kNearestBegin,
		integer kNearestEnd,
		Array<typename PointKdTree<Real, N, ObjectPolicy>::ConstObjectIterator, 2>* nearestArray,
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
		Euclidean_NormBijection<Real>(),
		DepthFirst_SearchAlgorithm_PointKdTree());
	*/
	template <typename Real, int N, typename ObjectPolicy,
		typename ConstObjectIterator_Iterator, 
		typename Real_Iterator>
	void searchAllNeighbors(
		const PointKdTree<Real, N, ObjectPolicy>& kdTree,
		const RandomAccessRange<ConstObjectIterator_Iterator>& querySet,
		integer kNearestBegin,
		integer kNearestEnd,
		Array<typename PointKdTree<Real, N, ObjectPolicy>::ConstObjectIterator, 2>* nearestArray,
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
		maxDistanceSet, 0,
		Euclidean_NormBijection<Real>(),
		DepthFirst_SearchAlgorithm_PointKdTree());
	*/
	template <typename Real, int N, typename ObjectPolicy,
		typename ConstObjectIterator_Iterator, 
		typename Real_Iterator>
	void searchAllNeighbors(
		const PointKdTree<Real, N, ObjectPolicy>& kdTree,
		const RandomAccessRange<ConstObjectIterator_Iterator>& querySet,
		integer kNearestBegin,
		integer kNearestEnd,
		Array<typename PointKdTree<Real, N, ObjectPolicy>::ConstObjectIterator, 2>* nearestArray,
		Array<PASTEL_NO_DEDUCTION(Real), 2>* distanceArray,
		const RandomAccessRange<Real_Iterator>& maxDistanceSet);

	//! Finds k nearest-neighbours for the given query points.
	/*!
	This is a convenience function that calls:
	searchAllNeighbors(
		kdTree, querySet,
		kNearestBegin, kNearestEnd,
		nearestArray, distanceArray,
		randomAccessRange(constantIterator<Real>(infinity<Real>()), querySet.size()));
		maxDistanceSet, 0,
		Euclidean_NormBijection<Real>(),
		DepthFirst_SearchAlgorithm_PointKdTree());
	*/
	template <typename Real, int N, typename ObjectPolicy,
		typename ConstObjectIterator_Iterator>
	void searchAllNeighbors(
		const PointKdTree<Real, N, ObjectPolicy>& kdTree,
		const RandomAccessRange<ConstObjectIterator_Iterator>& querySet,
		integer kNearestBegin,
		integer kNearestEnd,
		Array<typename PointKdTree<Real, N, ObjectPolicy>::ConstObjectIterator, 2>* nearestArray,
		Array<PASTEL_NO_DEDUCTION(Real), 2>* distanceArray);

	//! Finds k nearest-neighbours for the given query points.
	/*!
	This is a convenience function that calls:
	searchAllNeighbors(
		kdTree, querySet,
		kNearestBegin, kNearestEnd,
		nearestArray, 0,
		randomAccessRange(constantIterator<Real>(infinity<Real>()), querySet.size()));
		maxDistanceSet, 0,
		Euclidean_NormBijection<Real>(),
		DepthFirst_SearchAlgorithm_PointKdTree());
	*/
	template <typename Real, int N, typename ObjectPolicy,
		typename ConstObjectIterator_Iterator>
	void searchAllNeighbors(
		const PointKdTree<Real, N, ObjectPolicy>& kdTree,
		const RandomAccessRange<ConstObjectIterator_Iterator>& querySet,
		integer kNearestBegin,
		integer kNearestEnd,
		Array<typename PointKdTree<Real, N, ObjectPolicy>::ConstObjectIterator, 2>* nearestArray);

}

#include "pastel/geometry/search_all_neighbors_pointkdtree.hpp"

#endif
