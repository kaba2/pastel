#ifndef PASTEL_COUNT_ALL_RANGE_POINTKDTREE_HPP
#define PASTEL_COUNT_ALL_RANGE_POINTKDTREE_HPP

#include "pastel/geometry/count_all_range_pointkdtree.h"
#include "pastel/geometry/count_range_pointkdtree.h"
#include "pastel/geometry/pointkdtree_tools.h"

#include "pastel/sys/pastelomp.h"
#include "pastel/sys/ensure.h"

namespace Pastel
{

	template <typename Real, int N, typename PointPolicy,
		typename ConstPointIterator_Iterator,
		typename Real_Iterator,
		typename Integer_OutputIterator>
	void countAllRange(
		const PointKdTree<Real, N, PointPolicy>& kdTree,
		const RandomAccessRange<ConstPointIterator_Iterator>& querySet,
		const RandomAccessRange<Real_Iterator>& maxDistanceSet,
		Integer_OutputIterator result,
		integer bucketSize)
	{
		ENSURE_OP(querySet.size(), ==, maxDistanceSet.size());
		ENSURE_OP(bucketSize, >=, 1);

		if (kdTree.empty() || querySet.empty())
		{
			// Nothing to compute.
			return;
		}

		const integer queries = querySet.size();
		const integer dimension = kdTree.dimension();

		typedef typename PointKdTree<Real, N, PointPolicy>::ConstPointIterator
			ConstPointIterator;

#		pragma omp parallel
		{

		AlignedBox<Real, N> queryRange(ofDimension(dimension));

#		pragma omp for
		for (integer i = 0;i < queries;++i)
		{
			PENSURE_OP(maxDistanceSet[i], >=, 0);

			if (maxDistanceSet[i] == 0)
			{
				result[i] = 0;
			}
			else
			{
				queryRange.min() = 
					kdTree.pointPolicy(querySet[i]->point()) - maxDistanceSet[i];
				queryRange.max() = 
					kdTree.pointPolicy(querySet[i]->point()) + maxDistanceSet[i];

				result[i] = countRange(
					kdTree, 
					queryRange,
					bucketSize);
			}
		}
		}
	}

}

#endif
