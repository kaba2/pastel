#ifndef PASTEL_COUNT_ALL_NEIGHBORS_POINTKDTREE_HPP
#define PASTEL_COUNT_ALL_NEIGHBORS_POINTKDTREE_HPP

#include "pastel/geometry/count_all_neighbors_pointkdtree.h"
#include "pastel/geometry/count_nearest_pointkdtree.h"
#include "pastel/geometry/pointkdtree_tools.h"

#include "pastel/math/euclidean_normbijection.h"

#include "pastel/sys/pastelomp.h"
#include "pastel/sys/ensure.h"

namespace Pastel
{

	template <typename Real, int N, typename ObjectPolicy,
		typename ConstObjectIterator_Iterator,
		typename Real_Iterator,
		typename Integer_OutputIterator,
		typename NormBijection>
	void countAllNeighbors(
		const PointKdTree<Real, N, ObjectPolicy>& kdTree,
		const RandomAccessRange<ConstObjectIterator_Iterator>& querySet,
		const RandomAccessRange<Real_Iterator>& maxDistanceSet,
		Integer_OutputIterator result,
		integer bucketSize,
		const NormBijection& normBijection)
	{
		ENSURE_OP(querySet.size(), ==, maxDistanceSet.size());
		ENSURE_OP(bucketSize, >, 0);

		if (kdTree.empty() || querySet.empty())
		{
			// Nothing to compute.
			return;
		}

		const integer queries = querySet.size();
		const integer dimension = kdTree.dimension();

		typedef typename PointKdTree<Real, N, ObjectPolicy>::ConstObjectIterator
			ConstObjectIterator;

#		pragma omp parallel for
		for (integer i = 0;i < queries;++i)
		{
			PENSURE_OP(maxDistanceSet[i], >=, 0);

			const Vector<Real, N> queryPoint(ofDimension(dimension), 
				withAliasing((Real*)kdTree.objectPolicy().point(querySet[i]->object())));

			result[i] = countNearest(
				kdTree, 
				queryPoint, 
				maxDistanceSet[i], 
				Always_AcceptPoint<ConstObjectIterator>(),
				bucketSize,
				normBijection);
		}
	}

	template <typename Real, int N, typename ObjectPolicy,
		typename ConstObjectIterator_Iterator,
		typename Real_Iterator,
		typename Integer_OutputIterator>
	void countAllNeighbors(
		const PointKdTree<Real, N, ObjectPolicy>& kdTree,
		const RandomAccessRange<ConstObjectIterator_Iterator>& querySet,
		const RandomAccessRange<Real_Iterator>& maxDistanceSet,
		Integer_OutputIterator result)
	{
		Pastel::countAllNeighbors(
			kdTree, querySet,
			maxDistanceSet, result,
			bucketSize,
			Euclidean_NormBijection<Real>());
	}

}


#endif
