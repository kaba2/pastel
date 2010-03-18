#ifndef PASTEL_SEARCH_ALL_NEIGHBORS_POINTKDTREE_HPP
#define PASTEL_SEARCH_ALL_NEIGHBORS_POINTKDTREE_HPP

#include "pastel/geometry/search_all_neighbors_pointkdtree.h"
#include "pastel/geometry/search_all_neighbors_1d.h"
#include "pastel/geometry/pointkdtree_tools.h"
#include "pastel/geometry/dont_acceptpoint.h"

#include "pastel/math/euclidean_normbijection.h"

#include "pastel/sys/pastelomp.h"
#include "pastel/sys/countingiterator.h"
#include "pastel/sys/constantiterator.h"
#include "pastel/sys/stdext_copy_n.h"

namespace Pastel
{

	template <typename Real, int N, typename ObjectPolicy,
		typename ConstObjectIterator_Iterator, 
		typename Real_Iterator,
		typename NormBijection,
		typename Real_Iterator_Hint,
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
		const RandomAccessRange<Real_Iterator_Hint>& hintDistanceSet,
		const SearchAlgorithm& searchAlgorithm)
	{
		ENSURE_OP(kNearestBegin, >=, 0);
		ENSURE_OP(kNearestBegin, <=, kNearestEnd);
		ENSURE_OP(maxRelativeError, >=, 0);
		ENSURE_OP(maxDistanceSet.size(), >=, querySet.size());
		ENSURE_OP(hintDistanceSet.size(), >=, querySet.size());

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

		typedef PointKdTree<Real, N, ObjectPolicy> KdTree;
		typedef typename KdTree::ConstObjectIterator ConstObjectIterator;

#		pragma omp parallel
		{
		typedef std::vector<ConstObjectIterator> NearestSet;
		typedef std::vector<Real> DistanceSet;
		NearestSet nearestSet(kNearestEnd);
		DistanceSet distanceSet(kNearestEnd);
		const integer queries = querySet.size();

#		pragma omp for 
		for (integer i = 0;i < queries;++i)
		{
			ENSURE_OP(maxDistanceSet[i], >=, 0);
			ENSURE_OP(hintDistanceSet[i], >=, 0);

			integer nearestCount = 0;

			Vector<Real, N> queryPoint(
				ofDimension(kdTree.dimension()), 
				withAliasing((Real*)kdTree.objectPolicy().point(querySet[i]->object())));

			if (hintDistanceSet[i] < maxDistanceSet[i])
			{
				nearestCount = 
					searchNearest(
					kdTree, queryPoint, 
					kNearestEnd,
					nearestSet.begin(), distanceSet.begin(),
					hintDistanceSet[i], maxRelativeError,
					Dont_AcceptPoint<ConstObjectIterator>(querySet[i]),
					normBijection, 
					searchAlgorithm);
			}

			if (nearestCount < kNearestEnd)
			{
				// Either the hint distance was larger
				// than the maximum distance or the
				// hint failed. In any case, perform a 
				// search with the maximum distance.

				nearestCount = 
					searchNearest(
					kdTree, queryPoint, 
					kNearestEnd,
					nearestSet.begin(), distanceSet.begin(),
					maxDistanceSet[i], maxRelativeError,
					Dont_AcceptPoint<ConstObjectIterator>(querySet[i]),
					normBijection, 
					searchAlgorithm);
			}

			if (nearestArray)
			{
				if (nearestCount > kNearestBegin)
				{
					std::copy(
						nearestSet.begin() + kNearestBegin,
						nearestSet.begin() + nearestCount,
						nearestArray->rowBegin(i));
				}
				const integer fillStart = std::max(nearestCount - kNearestBegin, 0);
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
				const integer fillStart = std::max(nearestCount - kNearestBegin, 0);
				std::fill(
					distanceArray->rowBegin(i) + fillStart,
					distanceArray->rowEnd(i), infinity<Real>());
			}
		}

		}
	}

	template <typename Real, int N, typename ObjectPolicy,
		typename ConstObjectIterator_Iterator, 
		typename Real_Iterator,
		typename NormBijection,
		typename Real_Iterator_Hint>
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
		const RandomAccessRange<Real_Iterator_Hint>& hintDistanceSet)
	{
		Pastel::searchAllNeighbors(
			kdTree, querySet,
			kNearestBegin, kNearestEnd,
			nearestArray, distanceArray,
			maxDistanceSet, maxRelativeError,
			normBijection, hintDistanceSet,
			BestFirst_SearchAlgorithm_PointKdTree());
	}

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
		const NormBijection& normBijection)
	{
		Pastel::searchAllNeighbors(
			kdTree, querySet,
			kNearestBegin, kNearestEnd,
			nearestArray, distanceArray,
			maxDistanceSet, maxRelativeError,
			normBijection,
			constantRange(infinity<Real>(), querySet.size()));
	}

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
		const PASTEL_NO_DEDUCTION(Real)& maxRelativeError)
	{
		Pastel::searchAllNeighbors(
			kdTree, querySet,
			kNearestBegin, kNearestEnd,
			nearestArray, distanceArray,
			maxDistanceSet, maxRelativeError,
			Euclidean_NormBijection<Real>());
	}

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
		const RandomAccessRange<Real_Iterator>& maxDistanceSet)
	{
		Pastel::searchAllNeighbors(
			kdTree, querySet,
			kNearestBegin, kNearestEnd,
			nearestArray, distanceArray,
			maxDistanceSet, 0);
	}

	template <typename Real, int N, typename ObjectPolicy,
		typename ConstObjectIterator_Iterator>
	void searchAllNeighbors(
		const PointKdTree<Real, N, ObjectPolicy>& kdTree,
		const RandomAccessRange<ConstObjectIterator_Iterator>& querySet,
		integer kNearestBegin,
		integer kNearestEnd,
		Array<typename PointKdTree<Real, N, ObjectPolicy>::ConstObjectIterator, 2>* nearestArray,
		Array<PASTEL_NO_DEDUCTION(Real), 2>* distanceArray)
	{
		Pastel::searchAllNeighbors(
			kdTree, querySet,
			kNearestBegin, kNearestEnd,
			nearestArray, distanceArray,
			constantRange(infinity<Real>(), querySet.size()));
	}

	template <typename Real, int N, typename ObjectPolicy,
		typename ConstObjectIterator_Iterator>
	void searchAllNeighbors(
		const PointKdTree<Real, N, ObjectPolicy>& kdTree,
		const RandomAccessRange<ConstObjectIterator_Iterator>& querySet,
		integer kNearestBegin,
		integer kNearestEnd,
		Array<typename PointKdTree<Real, N, ObjectPolicy>::ConstObjectIterator, 2>* nearestArray)
	{
		Pastel::searchAllNeighbors(
			kdTree, querySet,
			kNearestBegin, kNearestEnd,
			nearestArray, 0);
	}

}

#endif
