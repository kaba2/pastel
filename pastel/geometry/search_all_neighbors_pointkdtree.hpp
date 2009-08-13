#ifndef PASTEL_SEARCH_ALL_NEIGHBORS_POINTKDTREE_HPP
#define PASTEL_SEARCH_ALL_NEIGHBORS_POINTKDTREE_HPP

#include "pastel/geometry/search_all_neighbors_pointkdtree.h"
#include "pastel/geometry/search_all_neighbors_1d.h"
#include "pastel/geometry/pointkdtree_tools.h"

#include "pastel/sys/pastelomp.h"
#include "pastel/sys/countingiterator.h"
#include "pastel/sys/stdext_copy_n.h"

namespace Pastel
{

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
		Array<Real, 2>* distanceArray)
	{
		ENSURE_OP(kNearestBegin, >=, 0);
		ENSURE_OP(kNearestBegin, <=, kNearestEnd);
		ENSURE_OP(maxRelativeError, >=, 0);
		ENSURE_OP(querySet.size(), ==, maxDistanceSet.size());

		const integer kNearest = kNearestEnd - kNearestBegin;

		ENSURE2(!nearestArray || nearestArray->width() == kNearest, 
			nearestArray->width(), kNearest);
		ENSURE2(!nearestArray || nearestArray->height() == querySet.size(), 
			nearestArray->height(), querySet.size());
		
		ENSURE(!nearestArray || !distanceArray ||
			allEqual(nearestArray->extent(), distanceArray->extent()));

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

//#		pragma omp for schedule(dynamic, 100)
#		pragma omp for
		for (integer i = 0;i < queries;++i)
		{
			ENSURE_OP(maxDistanceSet[i], >=, 0);

			searchNearest(
				kdTree, 
				querySet[i], 
				searchAlgorithm,
				Accept_Except<ConstObjectIterator>(querySet[i]),
				maxDistanceSet[i], 
				maxRelativeError,
				normBijection, 
				kNearestEnd,
				nearestSet.begin(), 
				distanceSet.begin());

			if (nearestArray)
			{
				std::copy(
					nearestSet.begin() + kNearestBegin,
					nearestSet.end(),
					nearestArray->rowBegin(i));
			}
			if (distanceArray)
			{
				std::copy(
					distanceSet.begin() + kNearestBegin,
					distanceSet.end(),
					distanceArray->rowBegin(i));
			}
		}
		}
	}

}

#endif
