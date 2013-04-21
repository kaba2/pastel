#ifndef PASTELGEOMETRY_SEARCH_ALL_NEIGHBORS_POINTKDTREE_HPP
#define PASTELGEOMETRY_SEARCH_ALL_NEIGHBORS_POINTKDTREE_HPP

#include "pastel/geometry/search_all_neighbors_pointkdtree.h"
#include "pastel/geometry/search_all_neighbors_1d.h"
#include "pastel/geometry/depthfirst_searchalgorithm_pointkdtree.h"
#include "pastel/geometry/pointkdtree_tools.h"
#include "pastel/geometry/allexcept_indicator.h"

#include "pastel/math/euclidean_normbijection.h"

#include "pastel/sys/pastelomp.h"
#include "pastel/sys/counting_iterator.h"
#include "pastel/sys/constant_iterator.h"
#include "pastel/sys/copy_n.h"
#include "pastel/sys/outputs.h"

namespace Pastel
{

	template <typename Real, int N, typename PointPolicy,
		typename Point_ConstIterator_RandomAccessRange, 
		typename Real_RandomAccessRange,
		typename NormBijection,
		typename SearchAlgorithm>
	void searchAllNeighbors(
		const PointKdTree<Real, N, PointPolicy>& kdTree,
		const Point_ConstIterator_RandomAccessRange& querySet,
		integer kNearestBegin,
		integer kNearestEnd,
		Array<typename PointKdTree<Real, N, PointPolicy>::Point_ConstIterator, 2>* nearestArray,
		Array<PASTEL_NO_DEDUCTION(Real), 2>* distanceArray,
		const Real_RandomAccessRange& maxDistanceSet,
		const PASTEL_NO_DEDUCTION(Real)& maxRelativeError,
		const NormBijection& normBijection,
		const SearchAlgorithm& searchAlgorithm)
	{
		ENSURE_OP(kNearestBegin, >=, 0);
		ENSURE_OP(kNearestBegin, <=, kNearestEnd);
		ENSURE_OP(maxRelativeError, >=, 0);
		ENSURE_OP(maxDistanceSet.size(), >=, querySet.size());

		const integer kNearest = kNearestEnd - kNearestBegin;

		ENSURE2(!nearestArray || nearestArray->width() >= kNearest, 
			nearestArray->width(), kNearest);
		ENSURE2(!nearestArray || nearestArray->height() >= querySet.size(), 
			nearestArray->height(), querySet.size());
		
		ENSURE2(!distanceArray || distanceArray->width() >= kNearest, 
			distanceArray->width(), kNearest);
		ENSURE2(!distanceArray || distanceArray->height() >= querySet.size(), 
			distanceArray->height(), querySet.size());

		if (kNearest == 0 || kdTree.empty() || querySet.empty() ||
			(!nearestArray && !distanceArray))
		{
			// Nothing to compute.
			return;
		}

		const integer bucketSize = 8;

		typedef PointKdTree<Real, N, PointPolicy> KdTree;
		typedef typename KdTree::Point_ConstIterator Point_ConstIterator;

#		pragma omp parallel
		{
		typedef std::vector<Point_ConstIterator> NearestSet;
		typedef std::vector<Real> DistanceSet;
		NearestSet nearestSet(kNearestEnd);
		DistanceSet distanceSet(kNearestEnd);
		const integer queries = querySet.size();

#		pragma omp for 
		for (integer i = 0;i < queries;++i)
		{
			ENSURE_OP(maxDistanceSet[i], >=, 0);

			integer nearestCount = 0;

			nearestCount = 
				searchNearest(
				kdTree, querySet[i], 
				kNearestEnd,
				rangeOutput(range(nearestSet.begin(), nearestSet.end())), 
				rangeOutput(range(distanceSet.begin(), distanceSet.end())),
				maxDistanceSet[i], maxRelativeError,
				dontIndicator(querySet[i]),
				bucketSize,
				normBijection, 
				searchAlgorithm);

			if (nearestArray)
			{
				if (nearestCount > kNearestBegin)
				{
					std::copy(
						nearestSet.begin() + kNearestBegin,
						nearestSet.begin() + nearestCount,
						nearestArray->rowBegin(i));
				}
				const integer fillStart = std::max(nearestCount - kNearestBegin, (integer)0);
				std::fill(
					nearestArray->rowBegin(i) + fillStart,
					nearestArray->rowEnd(i),
					kdTree.end());
			}
			if (distanceArray)
			{
				if (nearestCount > kNearestBegin)
				{
					std::copy(
						distanceSet.begin() + kNearestBegin,
						distanceSet.begin() + nearestCount,
						distanceArray->rowBegin(i));
				}
				const integer fillStart = std::max(nearestCount - kNearestBegin, (integer)0);
				std::fill(
					distanceArray->rowBegin(i) + fillStart,
					distanceArray->rowEnd(i), infinity<Real>());
			}
		}

		}
	}

	template <typename Real, int N, typename PointPolicy,
		typename Point_ConstIterator_RandomAccessRange, 
		typename Real_RandomAccessRange,
		typename NormBijection>
	void searchAllNeighbors(
		const PointKdTree<Real, N, PointPolicy>& kdTree,
		const Point_ConstIterator_RandomAccessRange& querySet,
		integer kNearestBegin,
		integer kNearestEnd,
		Array<typename PointKdTree<Real, N, PointPolicy>::Point_ConstIterator, 2>* nearestArray,
		Array<PASTEL_NO_DEDUCTION(Real), 2>* distanceArray,
		const Real_RandomAccessRange& maxDistanceSet,
		const PASTEL_NO_DEDUCTION(Real)& maxRelativeError,
		const NormBijection& normBijection)
	{
		Pastel::searchAllNeighbors(
			kdTree, querySet,
			kNearestBegin, kNearestEnd,
			nearestArray, distanceArray,
			maxDistanceSet, maxRelativeError,
			normBijection, 
			DepthFirst_SearchAlgorithm_PointKdTree());
	}

	template <typename Real, int N, typename PointPolicy,
		typename Point_ConstIterator_RandomAccessRange, 
		typename Real_RandomAccessRange>
	void searchAllNeighbors(
		const PointKdTree<Real, N, PointPolicy>& kdTree,
		const Point_ConstIterator_RandomAccessRange& querySet,
		integer kNearestBegin,
		integer kNearestEnd,
		Array<typename PointKdTree<Real, N, PointPolicy>::Point_ConstIterator, 2>* nearestArray,
		Array<PASTEL_NO_DEDUCTION(Real), 2>* distanceArray,
		const Real_RandomAccessRange& maxDistanceSet,
		const PASTEL_NO_DEDUCTION(Real)& maxRelativeError)
	{
		Pastel::searchAllNeighbors(
			kdTree, querySet,
			kNearestBegin, kNearestEnd,
			nearestArray, distanceArray,
			maxDistanceSet, maxRelativeError,
			Euclidean_NormBijection<Real>());
	}

	template <typename Real, int N, typename PointPolicy,
		typename Point_ConstIterator_RandomAccessRange, 
		typename Real_RandomAccessRange>
	void searchAllNeighbors(
		const PointKdTree<Real, N, PointPolicy>& kdTree,
		const Point_ConstIterator_RandomAccessRange& querySet,
		integer kNearestBegin,
		integer kNearestEnd,
		Array<typename PointKdTree<Real, N, PointPolicy>::Point_ConstIterator, 2>* nearestArray,
		Array<PASTEL_NO_DEDUCTION(Real), 2>* distanceArray,
		const Real_RandomAccessRange& maxDistanceSet)
	{
		Pastel::searchAllNeighbors(
			kdTree, querySet,
			kNearestBegin, kNearestEnd,
			nearestArray, distanceArray,
			maxDistanceSet, 0);
	}

	template <typename Real, int N, typename PointPolicy,
		typename Point_ConstIterator_RandomAccessRange>
	void searchAllNeighbors(
		const PointKdTree<Real, N, PointPolicy>& kdTree,
		const Point_ConstIterator_RandomAccessRange& querySet,
		integer kNearestBegin,
		integer kNearestEnd,
		Array<typename PointKdTree<Real, N, PointPolicy>::Point_ConstIterator, 2>* nearestArray,
		Array<PASTEL_NO_DEDUCTION(Real), 2>* distanceArray)
	{
		Pastel::searchAllNeighbors(
			kdTree, querySet,
			kNearestBegin, kNearestEnd,
			nearestArray, distanceArray,
			constantRange(infinity<Real>(), querySet.size()));
	}

	template <typename Real, int N, typename PointPolicy,
		typename Point_ConstIterator_RandomAccessRange>
	void searchAllNeighbors(
		const PointKdTree<Real, N, PointPolicy>& kdTree,
		const Point_ConstIterator_RandomAccessRange& querySet,
		integer kNearestBegin,
		integer kNearestEnd,
		Array<typename PointKdTree<Real, N, PointPolicy>::Point_ConstIterator, 2>* nearestArray)
	{
		// The cast below is needed because of a bug in GCC 4.4.5.
		Pastel::searchAllNeighbors(
			kdTree, querySet,
			kNearestBegin, kNearestEnd,
			nearestArray, (Array<Real>*)0);
	}

}

#endif
