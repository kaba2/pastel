#ifndef PASTEL_COUNT_ALL_RANGE_POINTKDTREE_HPP
#define PASTEL_COUNT_ALL_RANGE_POINTKDTREE_HPP

#include "pastel/geometry/count_all_range_pointkdtree.h"
#include "pastel/geometry/count_range_pointkdtree.h"
#include "pastel/geometry/pointkdtree_tools.h"

#include "pastel/sys/pastelomp.h"
#include "pastel/sys/ensure.h"

namespace Pastel
{

	template <typename Real, int N, typename ObjectPolicy,
		typename ConstObjectIterator_Iterator,
		typename Real_Iterator,
		typename Integer_OutputIterator>
	void countAllRange(
		const PointKdTree<Real, N, ObjectPolicy>& kdTree,
		const RandomAccessRange<ConstObjectIterator_Iterator>& querySet,
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

		typedef typename PointKdTree<Real, N, ObjectPolicy>::ConstObjectIterator
			ConstObjectIterator;

#		pragma omp parallel
		{

		AlignedBox<Real, N> queryRange(ofDimension(dimension));

#		pragma omp for
		for (integer i = 0;i < queries;++i)
		{
			PENSURE_OP(maxDistanceSet[i], >=, 0);

			const Vector<Real, N> queryPoint(ofDimension(dimension), 
				withAliasing((Real*)kdTree.objectPolicy().point(querySet[i]->object())));

			queryRange.min() = queryPoint - maxDistanceSet[i];
			queryRange.max() = queryPoint + maxDistanceSet[i];

			result[i] = countRange(
				kdTree, 
				queryRange,
				bucketSize);
		}
		}
	}

}

#endif
