#ifndef PASTEL_COUNT_ALL_NEIGHBORS_POINTKDTREE_HPP
#define PASTEL_COUNT_ALL_NEIGHBORS_POINTKDTREE_HPP

#include "pastel/geometry/count_all_neighbors_pointkdtree.h"
#include "pastel/geometry/pointkdtree_tools.h"

#include "pastel/sys/pastelomp.h"
#include "pastel/sys/ensure.h"

namespace Pastel
{

	template <typename Real, int N, typename ObjectPolicy,
		typename ConstObjectIterator_Iterator,
		typename Real_Iterator,
		typename NormBijection,
		typename Integer_OutputIterator>
	void countAllNeighbors(
		const PointKdTree<Real, N, ObjectPolicy>& kdTree,
		const RandomAccessRange<ConstObjectIterator_Iterator>& querySet,
		const RandomAccessRange<Real_Iterator>& maxDistanceSet,
		const NormBijection& normBijection,
		Integer_OutputIterator result)
	{
		ENSURE_OP(querySet.size(), ==, maxDistanceSet.size());

		if (kdTree.empty() || querySet.empty())
		{
			// Nothing to compute.
			return;
		}

		const integer queries = querySet.size();

		typedef typename PointKdTree<Real, N, ObjectPolicy>::ConstObjectIterator
			ConstObjectIterator;

		Always_AcceptPoint<ConstObjectIterator> acceptPoint;

#		pragma omp parallel for
		for (integer i = 0;i < queries;++i)
		{
			PENSURE(maxDistanceSet[i] >= 0);

			result[i] = countNearest(
				kdTree, 
				querySet[i], 
				maxDistanceSet[i], 
				acceptPoint,
				normBijection);
		}
	}

}


#endif
